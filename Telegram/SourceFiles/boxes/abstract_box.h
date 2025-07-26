/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include "ui/layers/box_layer_widget.h"

class Painter;

namespace style {
struct RoundButton;
struct IconButton;
struct ScrollArea;
} // namespace style

namespace Ui {
class RoundButton;
class IconButton;
class ScrollArea;
class FlatLabel;
class FadeShadow;
} // namespace Ui

// Legacy global method.
namespace Ui {
namespace internal {

void showBox(
	object_ptr<BoxContent> content,
	Ui::LayerOptions options,
	anim::type animated);

} // namespace internal

template <typename BoxType>
base::weak_qptr<BoxType> show(
		object_ptr<BoxType> content,
		Ui::LayerOptions options = Ui::LayerOption::CloseOther,
		anim::type animated = anim::type::normal) {
	auto result = base::weak_qptr<BoxType>(content.data());
	internal::showBox(std::move(content), options, animated);
	return result;
}

void hideLayer(anim::type animated = anim::type::normal);

} // namespace Ui
