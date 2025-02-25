/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include "styles/style_widgets.h"

namespace Ui {
class Show;
class RpWidget;
} // namespace Ui

namespace Ui::Toast {
class Instance;
} // namespace Ui::Toast

namespace Window {
class SessionController;
} // namespace Window

namespace HistoryView {

class StickerToast final {
public:
	StickerToast(
		not_null<Window::SessionController*> controller,
		not_null<QWidget*> parent,
		Fn<void()> destroy);
	~StickerToast();

	enum class Section {
		Message,
		TopicIcon,
	};
	void showFor(
		not_null<DocumentData*> document,
		Section section = Section::Message);

private:
	void requestSet();
	void cancelRequest();
	void showWithTitle(const QString &title);
	[[nodiscard]] QString lookupTitle() const;

	void setupEmojiPreview(not_null<Ui::RpWidget*> widget, int size);
	void setupLottiePreview(not_null<Ui::RpWidget*> widget, int size);
	void clearHiddenHiding();

	const not_null<Window::SessionController*> _controller;
	const not_null<QWidget*> _parent;
	Section _section = {};
	style::Toast _st;
	base::weak_ptr<Ui::Toast::Instance> _weak;
	std::vector<base::weak_ptr<Ui::Toast::Instance>> _hiding;
	DocumentData *_for = nullptr;
	Fn<void()> _destroy;

	mtpRequestId _setRequestId = 0;

};

} // namespace HistoryView
