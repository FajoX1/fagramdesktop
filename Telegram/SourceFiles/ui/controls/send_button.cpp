/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#include "ui/controls/send_button.h"

#include "ui/effects/ripple_animation.h"
#include "ui/painter.h"
#include "ui/ui_utility.h"
#include "styles/style_chat_helpers.h"

namespace Ui {
namespace {

constexpr int kWideScale = 5;

} // namespace

SendButton::SendButton(QWidget *parent, const style::SendButton &st)
: RippleButton(parent, st.inner.ripple)
, _st(st) {
	resize(_st.inner.width, _st.inner.height);
}

void SendButton::setType(Type type) {
	Expects(isSlowmode() || type != Type::Slowmode);

	if (isSlowmode() && type != Type::Slowmode) {
		_afterSlowmodeType = type;
		return;
	}
	if (_type != type) {
		_contentFrom = grabContent();
		_type = type;
		_a_typeChanged.stop();
		_contentTo = grabContent();
		_a_typeChanged.start(
			[=] { update(); },
			0.,
			1.,
			st::universalDuration);
		setPointerCursor(_type != Type::Slowmode);
		update();
	}
}

void SendButton::setSlowmodeDelay(int seconds) {
	Expects(seconds >= 0 && seconds < kSlowmodeDelayLimit);

	if (_slowmodeDelay == seconds) {
		return;
	}
	_slowmodeDelay = seconds;
	_slowmodeDelayText = isSlowmode()
		? u"%1:%2"_q.arg(seconds / 60).arg(seconds % 60, 2, 10, QChar('0'))
		: QString();
	setType(isSlowmode() ? Type::Slowmode : _afterSlowmodeType);
	update();
}

void SendButton::finishAnimating() {
	_a_typeChanged.stop();
	update();
}

void SendButton::paintEvent(QPaintEvent *e) {
	auto p = QPainter(this);

	auto over = (isDown() || isOver());
	auto changed = _a_typeChanged.value(1.);
	if (changed < 1.) {
		PainterHighQualityEnabler hq(p);
		p.setOpacity(1. - changed);
		auto targetRect = QRect((1 - kWideScale) / 2 * width(), (1 - kWideScale) / 2 * height(), kWideScale * width(), kWideScale * height());
		auto hiddenWidth = anim::interpolate(0, (1 - kWideScale) / 2 * width(), changed);
		auto hiddenHeight = anim::interpolate(0, (1 - kWideScale) / 2 * height(), changed);
		p.drawPixmap(targetRect.marginsAdded(QMargins(hiddenWidth, hiddenHeight, hiddenWidth, hiddenHeight)), _contentFrom);
		p.setOpacity(changed);
		auto shownWidth = anim::interpolate((1 - kWideScale) / 2 * width(), 0, changed);
		auto shownHeight = anim::interpolate((1 - kWideScale) / 2 * height(), 0, changed);
		p.drawPixmap(targetRect.marginsAdded(QMargins(shownWidth, shownHeight, shownWidth, shownHeight)), _contentTo);
		return;
	}
	switch (_type) {
	case Type::Record: paintRecord(p, over); break;
	case Type::Round: paintRound(p, over); break;
	case Type::Save: paintSave(p, over); break;
	case Type::Cancel: paintCancel(p, over); break;
	case Type::Send: paintSend(p, over); break;
	case Type::Schedule: paintSchedule(p, over); break;
	case Type::Slowmode: paintSlowmode(p); break;
	}
}

void SendButton::paintRecord(QPainter &p, bool over) {
	if (!isDisabled()) {
		paintRipple(
			p,
			(width() - _st.inner.rippleAreaSize) / 2,
			_st.inner.rippleAreaPosition.y());
	}

	const auto &icon = (isDisabled() || !over)
		? _st.record
		: _st.recordOver;
	icon.paintInCenter(p, rect());
}

void SendButton::paintRound(QPainter &p, bool over) {
	if (!isDisabled()) {
		paintRipple(
			p,
			(width() - _st.inner.rippleAreaSize) / 2,
			_st.inner.rippleAreaPosition.y());
	}

	const auto &icon = (isDisabled() || !over)
		? _st.round
		: _st.roundOver;
	icon.paintInCenter(p, rect());
}

void SendButton::paintSave(QPainter &p, bool over) {
	const auto &saveIcon = over
		? st::historyEditSaveIconOver
		: st::historyEditSaveIcon;
	saveIcon.paintInCenter(p, rect());
}

void SendButton::paintCancel(QPainter &p, bool over) {
	paintRipple(
		p,
		(width() - _st.inner.rippleAreaSize) / 2,
		_st.inner.rippleAreaPosition.y());

	const auto &cancelIcon = over
		? st::historyReplyCancelIconOver
		: st::historyReplyCancelIcon;
	cancelIcon.paintInCenter(p, rect());
}

void SendButton::paintSend(QPainter &p, bool over) {
	const auto &sendIcon = over ? _st.inner.iconOver : _st.inner.icon;
	if (isDisabled()) {
		const auto color = st::historyRecordVoiceFg->c;
		sendIcon.paint(p, st::historySendIconPosition, width(), color);
	} else {
		sendIcon.paint(p, st::historySendIconPosition, width());
	}
}

void SendButton::paintSchedule(QPainter &p, bool over) {
	{
		PainterHighQualityEnabler hq(p);
		p.setPen(Qt::NoPen);
		p.setBrush(over ? st::historySendIconFgOver : st::historySendIconFg);
		p.drawEllipse(
			st::historyScheduleIconPosition.x(),
			st::historyScheduleIconPosition.y(),
			st::historyScheduleIcon.width(),
			st::historyScheduleIcon.height());
	}
	st::historyScheduleIcon.paint(
		p,
		st::historyScheduleIconPosition,
		width());
}

void SendButton::paintSlowmode(QPainter &p) {
	p.setFont(st::normalFont);
	p.setPen(st::windowSubTextFg);
	p.drawText(
		rect().marginsRemoved(st::historySlowmodeCounterMargins),
		_slowmodeDelayText,
		style::al_center);
}

bool SendButton::isSlowmode() const {
	return (_slowmodeDelay > 0);
}

QPixmap SendButton::grabContent() {
	auto result = QImage(
		kWideScale * size() * style::DevicePixelRatio(),
		QImage::Format_ARGB32_Premultiplied);
	result.setDevicePixelRatio(style::DevicePixelRatio());
	result.fill(Qt::transparent);
	{
		auto p = QPainter(&result);
		p.drawPixmap(
			(kWideScale - 1) / 2 * width(),
			(kWideScale - 1) / 2 * height(),
			GrabWidget(this));
	}
	return Ui::PixmapFromImage(std::move(result));
}

QImage SendButton::prepareRippleMask() const {
	const auto size = _st.inner.rippleAreaSize;
	return RippleAnimation::EllipseMask(QSize(size, size));
}

QPoint SendButton::prepareRippleStartPosition() const {
	const auto real = mapFromGlobal(QCursor::pos());
	const auto size = _st.inner.rippleAreaSize;
	const auto y = (height() - _st.inner.rippleAreaSize) / 2;
	return real - QPoint((width() - size) / 2, y);
}

} // namespace Ui
