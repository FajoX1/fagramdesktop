/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

#include <QtCore/QString>
#include <map>

namespace Stripe {

class FormEncodable {
public:
	[[nodiscard]] virtual QString rootObjectName() = 0;
	[[nodiscard]] virtual std::map<QString, QString> formFieldValues() = 0;
};

template <typename T>
struct MakeEncodable final : FormEncodable {
public:
	MakeEncodable(const T &value) : _value(value) {
	}

	QString rootObjectName() override {
		return _value.rootObjectName();
	}
	std::map<QString, QString> formFieldValues() override {
		return _value.formFieldValues();
	}

private:
	const T &_value;

};

} // namespace Stripe
