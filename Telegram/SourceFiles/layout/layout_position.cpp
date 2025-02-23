/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#include "layout/layout_position.h"

namespace Layout {
namespace {

constexpr auto kMatrixRowShift = 40000;

} // namespace

Layout::Position IndexToPosition(int index) {
	return {
		(index >= 0) ? (index / kMatrixRowShift) : -1,
		(index >= 0) ? (index % kMatrixRowShift) : -1 };
}

int PositionToIndex(int row, int column) {
	return row * kMatrixRowShift + column;
}

int PositionToIndex(const Layout::Position &position) {
	return PositionToIndex(position.row, position.column);
}

} // namespace Layout
