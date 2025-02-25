/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include "stripe/stripe_card.h"
#include <vector>
#include <optional>

class QDate;

namespace Stripe {

enum class ValidationState {
	Invalid,
	Incomplete,
	Valid,
};

struct CardValidationResult {
	ValidationState state = ValidationState::Invalid;
	CardBrand brand = CardBrand::Unknown;
	bool finished = false;
};

[[nodiscard]] CardValidationResult ValidateCard(const QString &number);

struct ExpireDateValidationResult {
	ValidationState state = ValidationState::Invalid;
	bool finished = false;
};

[[nodiscard]] ExpireDateValidationResult ValidateExpireDate(
	const QString &date,
	const std::optional<QDate> &overrideExpireDateThreshold);

[[nodiscard]] ValidationState ValidateParsedExpireDate(
	quint32 month,
	quint32 year,
	const std::optional<QDate> &overrideExpireDateThreshold);

struct CvcValidationResult {
	ValidationState state = ValidationState::Invalid;
	bool finished = false;
};

[[nodiscard]] CvcValidationResult ValidateCvc(
	const QString &number,
	const QString &cvc);

[[nodiscard]] std::vector<int> CardNumberFormat(const QString &number);

} // namespace Stripe
