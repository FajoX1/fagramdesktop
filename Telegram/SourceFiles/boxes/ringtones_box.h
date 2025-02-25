/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include "ui/layers/generic_box.h"

namespace Data {
struct NotifySound;
class Thread;
} // namespace Data

namespace Main {
class Session;
} // namespace Main

namespace Ui {
class GenericBox;
} // namespace Ui

[[nodiscard]] QString ExtractRingtoneName(not_null<DocumentData*> document);

void RingtonesBox(
	not_null<Ui::GenericBox*> box,
	not_null<Main::Session*> session,
	Data::NotifySound selected,
	Fn<void(Data::NotifySound)> save);

void ThreadRingtonesBox(
	not_null<Ui::GenericBox*> box,
	not_null<Data::Thread*> thread);
