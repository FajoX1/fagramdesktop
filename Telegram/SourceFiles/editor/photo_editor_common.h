/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

namespace Editor {

class Scene;

struct PhotoModifications {
	int angle = 0;
	bool flipped = false;
	QRect crop;
	std::shared_ptr<Scene> paint = nullptr;

	[[nodiscard]] bool empty() const;
	[[nodiscard]] explicit operator bool() const;
	~PhotoModifications();

};

struct EditorData {
	enum class CropType {
		Rect,
		Ellipse,
		RoundedRect,
	};

	TextWithEntities about;
	QString confirm;
	QSize exactSize;
	CropType cropType = CropType::Rect;
	bool keepAspectRatio = false;
};

[[nodiscard]] QImage ImageModified(
	QImage image,
	const PhotoModifications &mods);

} // namespace Editor
