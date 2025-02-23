/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include "profile/profile_block_widget.h"
#include "ui/userpic_view.h"

namespace Ui {
class RippleAnimation;
class PopupMenu;
} // namespace Ui

namespace style {
struct PeerListItem;
} // namespace style

namespace Profile {

class PeerListWidget : public BlockWidget {
public:
	PeerListWidget(QWidget *parent, PeerData *peer, const QString &title, const style::PeerListItem &st, const QString &removeText);

	struct Item {
		explicit Item(not_null<PeerData*> peer);
		~Item();

		const not_null<PeerData*> peer;
		Ui::PeerUserpicView userpic;
		Ui::Text::String name;
		QString statusText;
		bool statusHasOnlineColor = false;
		std::optional<QString> rank;
		int rankWidth = 0;
		bool hasRemoveLink = false;
		std::unique_ptr<Ui::RippleAnimation> ripple;
	};
	int getListTop() const {
		return contentTop();
	}

	int getListLeft() const;

	const std::vector<Item*> &items() const {
		return _items;
	}
	int itemsCount() const {
		return _items.size();
	}

	// Does not take ownership of item.
	void addItem(Item *item) {
		if (!item) return;
		_items.push_back(item);
	}
	void clearItems() {
		_items.clear();
	}
	void reserveItemsForSize(int size) {
		_items.reserve(size);
	}
	template <typename Predicate>
	void sortItems(Predicate predicate) {
		std::sort(_items.begin(), _items.end(), std::move(predicate));
	}

	void setPreloadMoreCallback(Fn<void()> callback) {
		_preloadMoreCallback = std::move(callback);
	}
	void setSelectedCallback(Fn<void(PeerData*)> callback) {
		_selectedCallback = std::move(callback);
	}
	void setRemovedCallback(Fn<void(PeerData*)> callback) {
		_removedCallback = std::move(callback);
	}
	void setUpdateItemCallback(Fn<void(Item*)> callback) {
		_updateItemCallback = std::move(callback);
	}

protected:
	int resizeGetHeight(int newWidth) override;
	void visibleTopBottomUpdated(
		int visibleTop,
		int visibleBottom) override;

	void paintItemRect(Painter &p, int x, int y, int w, int h) const;
	void refreshVisibility();

	void paintContents(Painter &p) override;

	void mouseMoveEvent(QMouseEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	void enterEventHook(QEnterEvent *e) override;
	void enterFromChildEvent(QEvent *e, QWidget *child) override {
		enterEventHook(nullptr);
	}
	void leaveEventHook(QEvent *e) override;
	void leaveToChildEvent(QEvent *e, QWidget *child) override {
		leaveEventHook(e);
	}

private:
	void mousePressReleased(Qt::MouseButton button);
	void updateSelection();
	void setSelected(int selected, bool selectedRemove);
	void repaintSelectedRow();
	void repaintRow(int index);
	void preloadPhotos();
	int rowWidth() const;

	void paintItem(Painter &p, int x, int y, Item *item, bool selected, bool selectedRemove);

	const style::PeerListItem &_st;

	Fn<void()> _preloadMoreCallback;
	Fn<void(PeerData*)> _selectedCallback;
	Fn<void(PeerData*)> _removedCallback;
	Fn<void(Item*)> _updateItemCallback;

	std::vector<Item*> _items;

	int _visibleTop = 0;
	int _visibleBottom = 0;

	int _selected = -1;
	int _pressed = -1;
	Qt::MouseButton _pressButton = Qt::LeftButton;
	bool _selectedRemove = false;
	bool _pressedRemove = false;
	QPoint _mousePosition;

	QString _removeText;
	int _removeWidth = 0;

};

} // namespace Profile
