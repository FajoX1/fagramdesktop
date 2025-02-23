/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#import <AppKit/NSTouchBar.h>

namespace Window {
class Controller;
} // namespace Window

namespace TouchBar::Main {

const auto kPinnedPanelItemIdentifier = @"pinnedPanel";
const auto kPopoverInputItemIdentifier = @"popoverInput";
const auto kPopoverPickerItemIdentifier = @"pickerButtons";

} // namespace TouchBar::Main

API_AVAILABLE(macos(10.12.2))
@interface TouchBarMain : NSTouchBar
- (id)init:(not_null<Window::Controller*>)controller
	touchBarSwitches:(rpl::producer<>)touchBarSwitches;
@end
