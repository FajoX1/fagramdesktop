/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/

#include <ui/boxes/single_choice_box.h>

#include "fa/settings/fa_settings.h"
#include "fa/settings_menu/sections/fa_general.h"

#include "fa/lang/fa_lang.h"

#include "lang_auto.h"
#include "mainwindow.h"
#include "settings/settings_common.h"
#include "ui/wrap/vertical_layout.h"
#include "ui/wrap/slide_wrap.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/widgets/checkbox.h"
#include "ui/vertical_list.h"
#include "ui/boxes/confirm_box.h"
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
    FAlang::RplTranslate(QString(#LangKey)), \
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

#define RestartSettingsMenuJsonSwitch(LangKey, Option) container->add(object_ptr<Button>( \
    container, \
    FAlang::RplTranslate(QString(#LangKey)), \
    st::settingsButtonNoIcon \
))->toggleOn( \
    rpl::single(::FASettings::JsonSettings::GetBool(#Option)) \
)->toggledValue( \
) | rpl::filter([](bool enabled) { \
    return (enabled != ::FASettings::JsonSettings::GetBool(#Option)); \
}) | rpl::start_with_next([=](bool enabled) { \
    ::FASettings::JsonSettings::Write(); \
    ::FASettings::JsonSettings::Set(#Option, enabled); \
    ::FASettings::JsonSettings::Write(); \
    controller->show(Ui::MakeConfirmBox({ \
        .text = FAlang::RplTranslate(QString("fa_setting_need_restart")), \
        .confirmed = [=] { \
            ::Core::Restart(); \
        }, \
        .confirmText = FAlang::RplTranslate(QString("fa_restart")) \
    })); \
}, container->lifetime());

namespace Settings {

    rpl::producer<QString> FAGeneral::title() {
        return FAlang::RplTranslate(QString("fa_general"));
    }

    FAGeneral::FAGeneral(
            QWidget *parent,
            not_null<Window::SessionController *> controller)
            : Section(parent) {
        setupContent(controller);
    }

    void FAGeneral::SetupGeneral(not_null<Ui::VerticalLayout *> container, not_null<Window::SessionController *> controller) {
        Ui::AddSubsectionTitle(container, FAlang::RplTranslate(QString("fa_general")));

		SettingsMenuJsonSwitch(fa_show_seconds_message, seconds_message);
        SettingsMenuJsonSwitch(fa_disable_ads, disable_ads);
        SettingsMenuJsonSwitch(fa_show_start_token, show_start_token);
        SettingsMenuJsonSwitch(fa_show_peer_ids, show_peer_id);
        SettingsMenuJsonSwitch(fa_show_dc_ids, show_dc_id);
        SettingsMenuJsonSwitch(fa_id_in_botapi_type, show_id_botapi);
        SettingsMenuJsonSwitch(fa_local_tg_premium, local_premium);
        SettingsMenuJsonSwitch(fa_show_registration_date, show_registration_date);
        SettingsMenuJsonSwitch(fa_hide_phone_in_settings, hide_phone_number);
        RestartSettingsMenuJsonSwitch(fa_hide_stories, hide_stories);
    }

    void FAGeneral::SetupFAGeneral(not_null<Ui::VerticalLayout *> container, not_null<Window::SessionController *> controller) {
		Ui::AddSkip(container);
    	SetupGeneral(container, controller);
    }

    void FAGeneral::setupContent(not_null<Window::SessionController *> controller) {
        const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);

        SetupFAGeneral(content, controller);

        Ui::ResizeFitChild(this, content);
    }
} // namespace Settings

// thanks rabbitGram