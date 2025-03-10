/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include "media/clip/media_clip_reader.h"
#include "ui/effects/animations.h"
#include "data/data_file_origin.h"
#include "ui/rp_widget.h"

namespace Data {
class PhotoMedia;
class DocumentMedia;
} // namespace Data

namespace Lottie {
class SinglePlayer;
} // namespace Lottie

namespace Window {

class SessionController;

class MediaPreviewWidget final : public Ui::RpWidget {
public:
	MediaPreviewWidget(
		QWidget *parent,
		not_null<Window::SessionController*> controller);

	void showPreview(
		Data::FileOrigin origin,
		not_null<DocumentData*> document);
	void showPreview(
		Data::FileOrigin origin,
		not_null<PhotoData*> photo);
	void hidePreview();

	~MediaPreviewWidget();

protected:
	void paintEvent(QPaintEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;

private:
	void validateGifAnimation();
	void startGifAnimation(const Media::Clip::ReaderPointer &gif);
	QSize currentDimensions() const;
	QPixmap currentImage() const;
	void createLottieIfReady(not_null<DocumentData*> document);
	void setupLottie();
	void startShow();
	void fillEmojiString();
	void resetGifAndCache();
	[[nodiscard]] QPoint innerPosition(QSize size) const;
	[[nodiscard]] QPoint outerPosition(QSize size) const;
	[[nodiscard]] QRect updateArea() const;

	not_null<Window::SessionController*> _controller;

	Ui::Animations::Simple _a_shown;
	bool _hiding = false;
	Data::FileOrigin _origin;
	PhotoData *_photo = nullptr;
	DocumentData *_document = nullptr;
	std::shared_ptr<Data::PhotoMedia> _photoMedia;
	std::shared_ptr<Data::DocumentMedia> _documentMedia;
	Media::Clip::ReaderPointer _gif, _gifThumbnail;
	bool _gifWithAlpha = false;
	crl::time _gifLastPosition = 0;
	std::unique_ptr<Lottie::SinglePlayer> _lottie;
	std::unique_ptr<Lottie::SinglePlayer> _effect;

	int _emojiSize;
	std::vector<not_null<EmojiPtr>> _emojiList;

	void clipCallback(Media::Clip::Notification notification);

	enum CacheStatus {
		CacheNotLoaded,
		CacheThumbLoaded,
		CacheLoaded,
	};
	mutable CacheStatus _cacheStatus = CacheNotLoaded;
	mutable QPixmap _cache;
	mutable QSize _cachedSize;

};

} // namespace Window
