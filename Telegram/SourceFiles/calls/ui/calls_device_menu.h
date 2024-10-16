/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include "base/unique_qptr.h"

namespace Webrtc {
class Environment;
struct DeviceResolvedId;
enum class DeviceType : uchar;
} // namespace Webrtc

namespace Ui {
class RpWidget;
class PopupMenu;
} // namespace Ui

namespace Calls {

struct DeviceSelection {
	Webrtc::DeviceType type;
	rpl::producer<Webrtc::DeviceResolvedId> chosen;
};

[[nodiscard]] base::unique_qptr<Ui::PopupMenu> MakeDeviceSelectionMenu(
	not_null<Ui::RpWidget*> parent,
	not_null<Webrtc::Environment*> environment,
	std::vector<DeviceSelection> types,
	Fn<void(Webrtc::DeviceType, QString)> choose);

} // namespace Calls
