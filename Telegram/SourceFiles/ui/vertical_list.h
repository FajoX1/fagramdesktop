/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include "ui/rect_part.h"

namespace style {
struct FlatLabel;
} // namespace style

namespace st {
extern const style::margins &defaultBoxDividerLabelPadding;
} // namespace st

namespace Ui {

class FlatLabel;
class VerticalLayout;

void AddSkip(not_null<Ui::VerticalLayout*> container);
void AddSkip(not_null<Ui::VerticalLayout*> container, int skip);
void AddDivider(not_null<Ui::VerticalLayout*> container);
not_null<Ui::FlatLabel*> AddDividerText(
	not_null<Ui::VerticalLayout*> container,
	rpl::producer<QString> text,
	const style::margins &margins = st::defaultBoxDividerLabelPadding,
	RectParts parts = RectPart::Top | RectPart::Bottom);
not_null<Ui::FlatLabel*> AddDividerText(
	not_null<Ui::VerticalLayout*> container,
	rpl::producer<TextWithEntities> text,
	const style::margins &margins = st::defaultBoxDividerLabelPadding,
	RectParts parts = RectPart::Top | RectPart::Bottom);
not_null<Ui::FlatLabel*> AddSubsectionTitle(
	not_null<Ui::VerticalLayout*> container,
	rpl::producer<QString> text,
	style::margins addPadding = {},
	const style::FlatLabel *st = nullptr);

} // namespace Ui
