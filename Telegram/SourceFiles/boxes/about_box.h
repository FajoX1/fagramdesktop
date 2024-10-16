/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include "ui/layers/box_content.h"

namespace Ui {
class LinkButton;
class FlatLabel;
} // namespace Ui

class AboutBox : public Ui::BoxContent {
public:
	AboutBox(QWidget*);

protected:
	void prepare() override;

	void resizeEvent(QResizeEvent *e) override;
	void keyPressEvent(QKeyEvent *e) override;

private:
	void showVersionHistory();

	object_ptr<Ui::LinkButton> _version;
	object_ptr<Ui::FlatLabel> _text1;
	object_ptr<Ui::FlatLabel> _text2;
	object_ptr<Ui::FlatLabel> _text3;

};

QString telegramFaqLink();
QString currentVersionText();
