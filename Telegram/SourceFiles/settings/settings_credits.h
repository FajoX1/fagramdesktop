/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include "settings/settings_type.h"

namespace Api {
class CreditsTopupOptions;
} // namespace Api

namespace Main {
class SessionShow;
} // namespace Main

namespace Settings {

[[nodiscard]] Type CreditsId();
[[nodiscard]] Type CurrencyId();

class BuyStarsHandler final : public base::has_weak_ptr {
public:
	BuyStarsHandler();
	~BuyStarsHandler();

	[[nodiscard]] Fn<void()> handler(
		std::shared_ptr<::Main::SessionShow> show,
		Fn<void()> paid = nullptr);
	[[nodiscard]] rpl::producer<bool> loadingValue() const;

private:
	std::unique_ptr<Api::CreditsTopupOptions> _api;
	rpl::variable<bool> _loading;
	rpl::lifetime _lifetime;

};

} // namespace Settings
