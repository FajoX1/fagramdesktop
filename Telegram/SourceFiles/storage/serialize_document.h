/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include "data/data_document.h"

namespace Serialize {

class Document {
public:
	struct StickerSetInfo {
		StickerSetInfo(uint64 setId, uint64 accessHash, QString shortName)
			: setId(setId)
			, accessHash(accessHash)
			, shortName(shortName) {
		}
		uint64 setId;
		uint64 accessHash;
		QString shortName;
	};

	static void writeToStream(QDataStream &stream, DocumentData *document);
	static DocumentData *readStickerFromStream(
		not_null<Main::Session*> session,
		int streamAppVersion,
		QDataStream &stream,
		const StickerSetInfo &info);
	static DocumentData *readFromStream(
		not_null<Main::Session*> session,
		int streamAppVersion,
		QDataStream &stream);
	static int sizeInStream(DocumentData *document);

private:
	static DocumentData *readFromStreamHelper(
		not_null<Main::Session*> session,
		int streamAppVersion,
		QDataStream &stream,
		const StickerSetInfo *info);

};

} // namespace Serialize
