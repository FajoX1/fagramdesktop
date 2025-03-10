/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

namespace Ui {
#if 0
class PopupMenu;
class RpWidget;
#endif
class Show;
class GenericBox;
} // namespace Ui

namespace TTLMenu {

struct Args {
	std::shared_ptr<Ui::Show> show;
	TimeId startTtl;
	rpl::producer<TextWithEntities> about;
	Fn<void(TimeId, Fn<void()>)> callback;
	bool hideDisable = false;
};

void TTLBox(not_null<Ui::GenericBox*> box, Args args);
#if 0
void FillTTLMenu(not_null<Ui::PopupMenu*> menu, Args args);

void SetupTTLMenu(
	not_null<Ui::RpWidget*> parent,
	rpl::producer<> triggers,
	Args args);
#endif

} // namespace TTLMenu
