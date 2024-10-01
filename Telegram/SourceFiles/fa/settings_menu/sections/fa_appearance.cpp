/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/

#include <ui/boxes/single_choice_box.h>

#include "fa/settings/fa_settings.h"
#include "fa/settings_menu/sections/fa_appearance.h"

#include "lang_auto.h"
#include "mainwindow.h"
#include "settings/settings_common.h"
#include "ui/wrap/vertical_layout.h"
#include "ui/wrap/slide_wrap.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/widgets/checkbox.h"
#include "ui/vertical_list.h"
#include "boxes/connection_box.h"
#include "platform/platform_specific.h"
#include "window/window_session_controller.h"
#include "lang/lang_instance.h"
#include "core/application.h"
#include "storage/localstorage.h"
#include "data/data_session.h"
#include "main/main_session.h"
#include "styles/style_settings.h"
#include "styles/style_layers.h"
#include "styles/style_menu_icons.h"
#include "apiwrap.h"
#include "api/api_blocked_peers.h"
#include "ui/widgets/continuous_sliders.h"

#define SettingsMenuJsonSwitch(LangKey, Option) container->add(object_ptr<Button>( \
	container, \
    rpl::single(QString(#LangKey)), \
	st::settingsButtonNoIcon \
))->toggleOn( \
	rpl::single(::FASettings::JsonSettings::GetBool(#Option)) \
)->toggledValue( \
) | rpl::filter([](bool enabled) { \
	return (enabled != ::FASettings::JsonSettings::GetBool(#Option)); \
}) | rpl::start_with_next([](bool enabled) { \
	::FASettings::JsonSettings::Write(); \
	::FASettings::JsonSettings::Set(#Option, enabled); \
	::FASettings::JsonSettings::Write(); \
}, container->lifetime());

namespace Settings {

    rpl::producer<QString> FAAppearance::title() {
        return rpl::single(QString("Appearance"));
    }

    FAAppearance::FAAppearance(
            QWidget *parent,
            not_null<Window::SessionController *> controller)
            : Section(parent) {
        setupContent(controller);
    }

    void FAAppearance::SetupAppearance(not_null<Ui::VerticalLayout *> container) {
        Ui::AddSubsectionTitle(container, rpl::single(QString("Appearance")));

		const auto updateUserpicRoundnessLabel = [=](int value) {
    		const auto radius = QString::number(value);
    		userpicRoundnessLabel->setText(QString("Rounding: %1").arg(radius));
    	};
    	const auto updateUserpicRoundness = [=](int value) {
    		updateUserpicRoundnessLabel(value);
			roundnessPreview->repaint();
    		::FASettings::JsonSettings::Set("roundness", value);
    		::FASettings::JsonSettings::Write();
    	};
    	userpicRoundnessSlider->resize(st::settingsAudioVolumeSlider.seekSize);
    	userpicRoundnessSlider->setPseudoDiscrete(
			51,
			[](int val) { return val; },
			::FASettings::JsonSettings::GetInt("roundness"),
			updateUserpicRoundness);
    	updateUserpicRoundnessLabel(::FASettings::JsonSettings::GetInt("roundness"));
    }

    void FAAppearance::SetupFAAppearance(not_null<Ui::VerticalLayout *> container, not_null<Window::SessionController *> controller) {
		Ui::AddSkip(container);
    	SetupAppearance(container);
    }

    void FAAppearance::setupContent(not_null<Window::SessionController *> controller) {
        const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);

        SetupFAAppearance(content, controller);

        Ui::ResizeFitChild(this, content);
    }
} // namespace Settings

// thanks rabbitGram