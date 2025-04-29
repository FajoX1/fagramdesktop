/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

template <typename Object>
class object_ptr;

namespace Main {
class Session;
} // namespace Main

namespace style {
struct FlatLabel;
} // namespace style

namespace Ui::Text {
struct MarkedContext;
} // namespace Ui::Text

namespace Ui {

class FlatLabel;

[[nodiscard]] object_ptr<Ui::FlatLabel> CreateLabelWithCustomEmoji(
	QWidget *parent,
	rpl::producer<TextWithEntities> &&text,
	Text::MarkedContext context,
	const style::FlatLabel &st);

} // namespace Ui
