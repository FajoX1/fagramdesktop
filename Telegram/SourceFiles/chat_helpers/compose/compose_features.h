/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

namespace ChatHelpers {

struct ComposeFeatures {
	bool likes : 1 = false;
	bool sendAs : 1 = true;
	bool ttlInfo : 1 = true;
	bool botCommandSend : 1 = true;
	bool silentBroadcastToggle : 1 = true;
	bool attachBotsMenu : 1 = true;
	bool inlineBots : 1 = true;
	bool megagroupSet : 1 = true;
	bool collectibleStatus : 1 = false;
	bool stickersSettings : 1 = true;
	bool openStickerSets : 1 = true;
	bool autocompleteHashtags : 1 = true;
	bool autocompleteMentions : 1 = true;
	bool autocompleteCommands : 1 = true;
	bool suggestStickersByEmoji : 1 = true;
	bool commonTabbedPanel : 1 = true;
};

} // namespace ChatHelpers
