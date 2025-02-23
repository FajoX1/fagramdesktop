/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

template <typename Object>
class object_ptr;

namespace Ui {
class BoxContent;
class Show;
} // namespace Ui

namespace style {
struct ReportBox;
} // namespace style

class PeerData;

[[nodiscard]] object_ptr<Ui::BoxContent> ReportProfilePhotoBox(
	not_null<PeerData*> peer,
	not_null<PhotoData*> photo);

void ShowReportMessageBox(
	std::shared_ptr<Ui::Show> show,
	not_null<PeerData*> peer,
	const std::vector<MsgId> &ids,
	const std::vector<StoryId> &stories,
	const style::ReportBox *stOverride = nullptr);
