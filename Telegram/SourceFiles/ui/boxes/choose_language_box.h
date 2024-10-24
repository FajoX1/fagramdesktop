/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

struct LanguageId;

namespace tr {
struct now_t;
} // namespace tr

namespace Ui {

class GenericBox;

[[nodiscard]] QString LanguageNameTranslated(const QString &twoLetterCode);
[[nodiscard]] QString LanguageNameLocal(LanguageId id);
[[nodiscard]] QString LanguageName(LanguageId id);
[[nodiscard]] QString LanguageNameNative(LanguageId id);

[[nodiscard]] rpl::producer<QString> TranslateBarTo(LanguageId id);
[[nodiscard]] QString TranslateMenuDont(tr::now_t, LanguageId id);

void ChooseLanguageBox(
	not_null<GenericBox*> box,
	rpl::producer<QString> title,
	Fn<void(std::vector<LanguageId>)> callback,
	std::vector<LanguageId> selected,
	bool multiselect,
	Fn<bool(LanguageId)> toggleCheck);

} // namespace Ui
