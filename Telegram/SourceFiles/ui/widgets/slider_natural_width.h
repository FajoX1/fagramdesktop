/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include "ui/widgets/discrete_sliders.h"

namespace Ui {

class CustomWidthSlider final : public SettingsSlider {
public:
	using Ui::SettingsSlider::SettingsSlider;
	void setNaturalWidth(int w) {
		_naturalWidth = w;
	}
	int naturalWidth() const override {
		return _naturalWidth;
	}

private:
	int _naturalWidth = 0;

};

} // namespace Ui
