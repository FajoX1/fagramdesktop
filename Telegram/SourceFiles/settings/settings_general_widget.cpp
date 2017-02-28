/*
This file is part of Telegram Desktop,
the official desktop version of Telegram messaging app, see https://telegram.org

Telegram Desktop is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

It is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

In addition, as a special exception, the copyright holders give permission
to link the code of portions of this program with the OpenSSL library.

Full license: https://github.com/telegramdesktop/tdesktop/blob/master/LICENSE
Copyright (c) 2014-2017 John Preston, https://desktop.telegram.org
*/
#include "stdafx.h"
#include "settings/settings_general_widget.h"

#include "styles/style_settings.h"
#include "lang.h"
#include "ui/effects/widget_slide_wrap.h"
#include "ui/widgets/checkbox.h"
#include "ui/widgets/buttons.h"
#include "localstorage.h"
#include "pspecific.h"
#include "mainwindow.h"
#include "application.h"
#include "boxes/languagebox.h"
#include "boxes/confirmbox.h"
#include "boxes/aboutbox.h"
#include "core/file_utilities.h"
#include "langloaderplain.h"
#include "autoupdater.h"

namespace Settings {

#ifndef TDESKTOP_DISABLE_AUTOUPDATE
UpdateStateRow::UpdateStateRow(QWidget *parent) : TWidget(parent)
, _check(this, lang(lng_settings_check_now))
, _restart(this, lang(lng_settings_update_now)) {
	connect(_check, SIGNAL(clicked()), this, SLOT(onCheck()));
	connect(_restart, SIGNAL(clicked()), this, SIGNAL(restart()));

	Sandbox::connect(SIGNAL(updateChecking()), this, SLOT(onChecking()));
	Sandbox::connect(SIGNAL(updateLatest()), this, SLOT(onLatest()));
	Sandbox::connect(SIGNAL(updateProgress(qint64, qint64)), this, SLOT(onDownloading(qint64, qint64)));
	Sandbox::connect(SIGNAL(updateFailed()), this, SLOT(onFailed()));
	Sandbox::connect(SIGNAL(updateReady()), this, SLOT(onReady()));

	_versionText = lng_settings_current_version_label(lt_version, currentVersionText());

	switch (Sandbox::updatingState()) {
	case Application::UpdatingDownload:
		setState(State::Download, true);
		setDownloadProgress(Sandbox::updatingReady(), Sandbox::updatingSize());
	break;
	case Application::UpdatingReady: setState(State::Ready, true); break;
	default: setState(State::None, true); break;
	}
}

int UpdateStateRow::resizeGetHeight(int newWidth) {
	auto labelWidth = [](const QString &label) {
		return st::linkFont->width(label) + st::linkFont->spacew;
	};
	auto checkLeft = (_state == State::Latest) ? labelWidth(lang(lng_settings_latest_installed)) : labelWidth(_versionText);
	auto restartLeft = labelWidth(lang(lng_settings_update_ready));

	_check->resizeToWidth(qMin(newWidth, _check->naturalWidth()));
	_check->moveToLeft(checkLeft, 0, newWidth);

	_restart->resizeToWidth(qMin(newWidth, _restart->naturalWidth()));
	_restart->moveToLeft(restartLeft, 0, newWidth);

	return _check->height();
}

void UpdateStateRow::paintEvent(QPaintEvent *e) {
	Painter p(this);

	auto text = ([this]() -> QString {
		switch (_state) {
		case State::Check: return lang(lng_settings_update_checking);
		case State::Latest: return lang(lng_settings_latest_installed);
		case State::Download: return _downloadText;
		case State::Ready: return lang(lng_settings_update_ready);
		case State::Fail: return lang(lng_settings_update_fail);
		default: return _versionText;
		}
	})();
	p.setFont(st::linkFont);
	p.setPen((_state == State::None) ? st::windowFg : st::settingsUpdateFg);
	p.drawTextLeft(0, 0, width(), text);
}

void UpdateStateRow::onCheck() {
	if (!cAutoUpdate()) return;

	setState(State::Check);
	cSetLastUpdateCheck(0);
	Sandbox::startUpdateCheck();
}

void UpdateStateRow::setState(State state, bool force) {
	if (_state != state || force) {
		_state = state;
		switch (state) {
		case State::None: _check->show(); _restart->hide(); break;
		case State::Ready: _check->hide(); _restart->show(); break;
		case State::Check:
		case State::Download:
		case State::Latest:
		case State::Fail: _check->hide(); _restart->hide(); break;
		}
		resizeToWidth(width());
		sendSynteticMouseEvent(this, QEvent::MouseMove, Qt::NoButton);
		update();
	}
}

void UpdateStateRow::setDownloadProgress(qint64 ready, qint64 total) {
	auto readyTenthMb = (ready * 10 / (1024 * 1024)), totalTenthMb = (total * 10 / (1024 * 1024));
	auto readyStr = QString::number(readyTenthMb / 10) + '.' + QString::number(readyTenthMb % 10);
	auto totalStr = QString::number(totalTenthMb / 10) + '.' + QString::number(totalTenthMb % 10);
	auto result = lng_settings_downloading(lt_ready, readyStr, lt_total, totalStr);
	if (_downloadText != result) {
		_downloadText = result;
		update();
	}
}

void UpdateStateRow::onChecking() {
	setState(State::Check);
}

void UpdateStateRow::onLatest() {
	setState(State::Latest);
}

void UpdateStateRow::onDownloading(qint64 ready, qint64 total) {
	setState(State::Download);
	setDownloadProgress(ready, total);
}

void UpdateStateRow::onReady() {
	setState(State::Ready);
}

void UpdateStateRow::onFailed() {
	setState(State::Fail);
}
#endif // !TDESKTOP_DISABLE_AUTOUPDATE

GeneralWidget::GeneralWidget(QWidget *parent, UserData *self) : BlockWidget(parent, self, lang(lng_settings_section_general))
, _changeLanguage(this, lang(lng_settings_change_lang), st::boxLinkButton) {
	connect(_changeLanguage, SIGNAL(clicked()), this, SLOT(onChangeLanguage()));
	subscribe(Global::RefChooseCustomLang(), [this]() { chooseCustomLang(); });
	subscribe(FileDialog::QueryDone(), [this](const FileDialog::QueryUpdate &update) {
		notifyFileQueryUpdated(update);
	});
	refreshControls();
}

int GeneralWidget::resizeGetHeight(int newWidth) {
	_changeLanguage->moveToRight(contentLeft(), st::settingsBlockMarginTop + st::settingsBlockTitleTop + st::settingsBlockTitleFont->ascent - st::defaultLinkButton.font->ascent, newWidth);
	return BlockWidget::resizeGetHeight(newWidth);
}

void GeneralWidget::refreshControls() {
	style::margins marginSub(0, 0, 0, st::settingsSubSkip);
	style::margins marginLarge(0, 0, 0, st::settingsLargeSkip);
	style::margins marginSmall(0, 0, 0, st::settingsSmallSkip);
	style::margins slidedPadding(0, marginSmall.bottom() / 2, 0, marginSmall.bottom() - (marginSmall.bottom() / 2));

#ifndef TDESKTOP_DISABLE_AUTOUPDATE
	addChildRow(_updateAutomatically, marginSub, lang(lng_settings_update_automatically), SLOT(onUpdateAutomatically()), cAutoUpdate());
	style::margins marginLink(st::defaultBoxCheckbox.textPosition.x(), 0, 0, st::settingsSkip);
	addChildRow(_updateRow, marginLink, slidedPadding);
	connect(_updateRow->entity(), SIGNAL(restart()), this, SLOT(onRestart()));
	if (!cAutoUpdate()) {
		_updateRow->hideFast();
	}
#endif // !TDESKTOP_DISABLE_AUTOUPDATE

	if (cPlatform() == dbipWindows || cSupportTray()) {
		addChildRow(_enableTrayIcon, marginSmall, lang(lng_settings_workmode_tray), SLOT(onEnableTrayIcon()), (cWorkMode() == dbiwmTrayOnly || cWorkMode() == dbiwmWindowAndTray));
		if (cPlatform() == dbipWindows) {
			addChildRow(_enableTaskbarIcon, marginLarge, lang(lng_settings_workmode_window), SLOT(onEnableTaskbarIcon()), (cWorkMode() == dbiwmWindowOnly || cWorkMode() == dbiwmWindowAndTray));

#ifndef OS_WIN_STORE
			addChildRow(_autoStart, marginSmall, lang(lng_settings_auto_start), SLOT(onAutoStart()), cAutoStart());
			addChildRow(_startMinimized, marginLarge, slidedPadding, lang(lng_settings_start_min), SLOT(onStartMinimized()), (cStartMinimized() && !Global::LocalPasscode()));
			subscribe(Global::RefLocalPasscodeChanged(), [this] {
				_startMinimized->entity()->setChecked(cStartMinimized() && !Global::LocalPasscode());
			});
			if (!cAutoStart()) {
				_startMinimized->hideFast();
			}
			addChildRow(_addInSendTo, marginSmall, lang(lng_settings_add_sendto), SLOT(onAddInSendTo()), cSendToMenu());
#endif // OS_WIN_STORE
		}
	}
}

void GeneralWidget::chooseCustomLang() {
	auto filter = qsl("Language files (*.strings)");
	auto title = qsl("Choose language .strings file");

	_chooseLangFileQueryId = FileDialog::queryReadFile(title, filter);
}

void GeneralWidget::notifyFileQueryUpdated(const FileDialog::QueryUpdate &update) {
	if (_chooseLangFileQueryId != update.queryId) {
		return;
	}
	_chooseLangFileQueryId = 0;

	if (update.filePaths.isEmpty()) {
		return;
	}

	_testLanguage = QFileInfo(update.filePaths.front()).absoluteFilePath();
	LangLoaderPlain loader(_testLanguage, langLoaderRequest(lng_sure_save_language, lng_cancel, lng_box_ok));
	if (loader.errors().isEmpty()) {
		LangLoaderResult result = loader.found();
		auto text = result.value(lng_sure_save_language, langOriginal(lng_sure_save_language)),
			save = result.value(lng_box_ok, langOriginal(lng_box_ok)),
			cancel = result.value(lng_cancel, langOriginal(lng_cancel));
		Ui::show(Box<ConfirmBox>(text, save, cancel, base::lambda_guarded(this, [this] {
			cSetLangFile(_testLanguage);
			cSetLang(languageTest);
			Local::writeSettings();
			onRestart();
		})));
	} else {
		Ui::show(Box<InformBox>("Custom lang failed :(\n\nError: " + loader.errors()));
	}
}

void GeneralWidget::onChangeLanguage() {
	if ((_changeLanguage->clickModifiers() & Qt::ShiftModifier) && (_changeLanguage->clickModifiers() & Qt::AltModifier)) {
		chooseCustomLang();
	} else {
		Ui::show(Box<LanguageBox>());
	}
}

void GeneralWidget::onRestart() {
#ifndef TDESKTOP_DISABLE_AUTOUPDATE
	checkReadyUpdate();
#endif // !TDESKTOP_DISABLE_AUTOUPDATE
	App::restart();
}

#ifndef TDESKTOP_DISABLE_AUTOUPDATE
void GeneralWidget::onUpdateAutomatically() {
	cSetAutoUpdate(_updateAutomatically->checked());
	Local::writeSettings();
	if (cAutoUpdate()) {
		_updateRow->slideDown();
		Sandbox::startUpdateCheck();
	} else {
		_updateRow->slideUp();
		Sandbox::stopUpdate();
	}
}
#endif // !TDESKTOP_DISABLE_AUTOUPDATE

void GeneralWidget::onEnableTrayIcon() {
	if ((!_enableTrayIcon->checked() || cPlatform() != dbipWindows) && _enableTaskbarIcon && !_enableTaskbarIcon->checked()) {
		_enableTaskbarIcon->setChecked(true);
	} else {
		updateWorkmode();
	}
}

void GeneralWidget::onEnableTaskbarIcon() {
	if (!_enableTrayIcon->checked() && !_enableTaskbarIcon->checked()) {
		_enableTrayIcon->setChecked(true);
	} else {
		updateWorkmode();
	}
}

void GeneralWidget::updateWorkmode() {
	DBIWorkMode newMode = (_enableTrayIcon->checked() && (!_enableTaskbarIcon || _enableTaskbarIcon->checked())) ? dbiwmWindowAndTray : (_enableTrayIcon->checked() ? dbiwmTrayOnly : dbiwmWindowOnly);
	if (cWorkMode() != newMode && (newMode == dbiwmWindowAndTray || newMode == dbiwmTrayOnly)) {
		cSetSeenTrayTooltip(false);
	}
	cSetWorkMode(newMode);
	App::wnd()->psUpdateWorkmode();
	Local::writeSettings();
}

#if !defined OS_WIN_STORE
void GeneralWidget::onAutoStart() {
	cSetAutoStart(_autoStart->checked());
	if (cAutoStart()) {
		psAutoStart(true);
		_startMinimized->slideDown();
		Local::writeSettings();
	} else {
		psAutoStart(false);
		if (_startMinimized->entity()->checked()) {
			_startMinimized->entity()->setChecked(false);
		} else {
			Local::writeSettings();
		}
		_startMinimized->slideUp();
	}
}

void GeneralWidget::onStartMinimized() {
	auto checked = _startMinimized->entity()->checked();
	if (Global::LocalPasscode()) {
		if (checked) {
			_startMinimized->entity()->setChecked(false);
			Ui::show(Box<InformBox>(lang(lng_error_start_minimized_passcoded)));
		}
		return;
	}
	if (cStartMinimized() != checked) {
		cSetStartMinimized(checked);
		Local::writeSettings();
	}
}

void GeneralWidget::onAddInSendTo() {
	cSetSendToMenu(_addInSendTo->checked());
	psSendToMenu(_addInSendTo->checked());
	Local::writeSettings();
}
#endif // !OS_WIN_STORE

} // namespace Settings
