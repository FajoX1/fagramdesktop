/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include "platform/platform_notifications_manager.h"

namespace Platform {
namespace Notifications {

class Manager : public Window::Notifications::NativeManager {
public:
	Manager(not_null<Window::Notifications::System*> system);
	~Manager();

protected:
	void doShowNativeNotification(
		NotificationInfo &&info,
		Ui::PeerUserpicView &userpicView) override;
	void doClearAllFast() override;
	void doClearFromItem(not_null<HistoryItem*> item) override;
	void doClearFromTopic(not_null<Data::ForumTopic*> topic) override;
	void doClearFromHistory(not_null<History*> history) override;
	void doClearFromSession(not_null<Main::Session*> session) override;
	bool doSkipToast() const override;
	void doMaybePlaySound(Fn<void()> playSound) override;
	void doMaybeFlashBounce(Fn<void()> flashBounce) override;

private:
	friend void Create(Window::Notifications::System *system);
	class Private;
	const std::unique_ptr<Private> _private;

};

} // namespace Notifications
} // namespace Platform
