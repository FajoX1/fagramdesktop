/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include "base/object_ptr.h"
#include "ui/layers/generic_box.h"

class PeerData;

namespace Main {
struct SendAsPeer;
} // namespace Main

namespace Window {
class SessionController;
} // namespace Window

namespace Ui {

class SendAsButton;

void ChooseSendAsBox(
	not_null<GenericBox*> box,
	std::vector<Main::SendAsPeer> list,
	not_null<PeerData*> chosen,
	Fn<bool(not_null<PeerData*>)> done);

void SetupSendAsButton(
	not_null<SendAsButton*> button,
	rpl::producer<PeerData*> active,
	not_null<Window::SessionController*> window);

void SetupSendAsButton(
	not_null<SendAsButton*> button,
	not_null<Window::SessionController*> window);

} // namespace Ui
