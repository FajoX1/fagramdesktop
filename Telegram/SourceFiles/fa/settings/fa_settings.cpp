/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/

#include "fa/settings/fa_settings.h"

#include "fa/fa_version.h"
#include "mainwindow.h"
#include "mainwidget.h"
#include "window/window_controller.h"
#include "core/application.h"
#include "data/data_peer_id.h"
#include "base/parse_helper.h"
#include "base/timer.h"
#include "data/data_chat_filters.h"
#include "platform/platform_file_utilities.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QTimer>

namespace FASettings {
namespace JsonSettings {
namespace {

constexpr auto kWriteJsonTimeout = crl::time(5000);

class Manager : public QObject {
public:
	Manager();
	void load();
	void fill();
	void write(bool force = false);

	[[nodiscard]] QVariant get(
		const QString &key,
		uint64 accountId = 0,
		bool isTestAccount = false);
	[[nodiscard]] QVariant getWithPending(
		const QString &key,
		uint64 accountId = 0,
		bool isTestAccount = false);
	[[nodiscard]] QVariantMap getAllWithPending(const QString &key);
	[[nodiscard]] rpl::producer<QString> events(
		const QString &key,
		uint64 accountId = 0,
		bool isTestAccount = false);
	[[nodiscard]] rpl::producer<QString> eventsWithPending(
		const QString &key,
		uint64 accountId = 0,
		bool isTestAccount = false);
	void set(
		const QString &key,
		QVariant value,
		uint64 accountId = 0,
		bool isTestAccount = false);
	void setAfterRestart(
		const QString &key,
		QVariant value,
		uint64 accountId = 0,
		bool isTestAccount = false);
	void reset(
		const QString &key,
		uint64 accountId = 0,
		bool isTestAccount = false);
	void resetAfterRestart(
		const QString &key,
		uint64 accountId = 0,
		bool isTestAccount = false);
	void writeTimeout();

private:
	[[nodiscard]] QVariant getDefault(const QString &key);

	void writeDefaultFile();
	void writeCurrentSettings();
	bool readCustomFile();
	void writing();

	base::Timer _jsonWriteTimer;

	rpl::event_stream<QString> _eventStream;
	rpl::event_stream<QString> _pendingEventStream;
	QHash<QString, QVariant> _settingsHashMap;
	QHash<QString, QVariant> _defaultSettingsHashMap;

};

inline QString MakeMapKey(const QString &key, uint64 accountId, bool isTestAccount) {
	return (accountId == 0)	? key : key
				+ (isTestAccount ? qsl(":test_") : qsl(":"))
				+ QString::number(accountId);
}

QVariantMap GetAllWithPending(const QString &key);

enum SettingScope {
	Global,
	Account,
};

enum SettingStorage {
	None,
	MainJson,
};

enum SettingType {
	BoolSetting,
	IntSetting,
	QStringSetting,
	QJsonArraySetting,
};

using CheckHandler = Fn<QVariant(QVariant)>;

CheckHandler IntLimit(int min, int max, int defaultValue) {
	return [=] (QVariant value) -> QVariant {
		if (value.canConvert<int>()) {
			auto intValue = value.toInt();
			if (intValue < min) {
				return min;
			} else if (intValue > max) {
				return max;
			} else {
				return value;
			}
		} else {
			return defaultValue;
		}
	};
}

inline CheckHandler IntLimit(int min, int max) {
	return IntLimit(min, max, min);
}

struct Definition {
	SettingScope scope = SettingScope::Global;
	SettingStorage storage = SettingStorage::MainJson;
	SettingType type = SettingType::BoolSetting;
	QVariant defaultValue;
	QVariant fillerValue;
	CheckHandler limitHandler = nullptr;
};

const std::map<QString, Definition, std::greater<QString>> DefinitionMap {

	// Stored settings
	//   General
	{ "debug_logs", {
		.type = SettingType::BoolSetting,
		.defaultValue = false, }},
	{ "seconds_message", {
		.type = SettingType::BoolSetting,
		.defaultValue = true, }},
	{ "disable_ads", {
		.type = SettingType::BoolSetting,
		.defaultValue = false, }},
	{ "show_start_token", {
		.type = SettingType::BoolSetting,
		.defaultValue = true, }},
	{ "show_peer_id", {
		.type = SettingType::BoolSetting,
		.defaultValue = true, }},
	{ "show_dc_id", {
		.type = SettingType::BoolSetting,
		.defaultValue = true, }},
	{ "show_id_botapi", {
		.type = SettingType::BoolSetting,
		.defaultValue = true, }},
	{ "show_registration_date", {
		.type = SettingType::BoolSetting,
		.defaultValue = true, }},
	{ "hide_phone_number", {
		.type = SettingType::BoolSetting,
		.defaultValue = false, }},
	{ "disable_custom_chat_background", {
		.type = SettingType::BoolSetting,
		.defaultValue = false, }},
	{ "hide_all_chats_folder", {
		.type = SettingType::BoolSetting,
		.defaultValue = false, }},
	{ "hide_stories", {
		.type = SettingType::BoolSetting,
		.defaultValue = false, }},
	{ "hide_open_webapp_button_chatlist", {
		.type = SettingType::BoolSetting,
		.defaultValue = true, }},
	{ "local_premium", {
		.type = SettingType::BoolSetting,
		.defaultValue = false, }},
	{ "recent_stickers_limit", {
		.type = SettingType::IntSetting,
		.defaultValue = 30,
		.limitHandler = IntLimit(0, 100, 30), }},
	{ "use_default_rounding", {
		.type = SettingType::BoolSetting,
		.defaultValue = true, }},
	{ "show_discuss_button", {
		.type = SettingType::BoolSetting,
		.defaultValue = true, }},
	{ "roundness", {
		.type = SettingType::IntSetting,
		.defaultValue = 50,
		.limitHandler = IntLimit(0, 50, 50), }},
	{ "force_snow", {
		.type = SettingType::BoolSetting,
		.defaultValue = false, }},
	{ "show_message_details", {
		.type = SettingType::BoolSetting,
		.defaultValue = true, }},
	{ "show_message_details", {
		.type = SettingType::BoolSetting,
		.defaultValue = true, }},
	{ "show_repeat_option", {
		.type = SettingType::BoolSetting,
		.defaultValue = true, }},
	{ "repeat_reply_to_orig_msg", {
		.type = SettingType::BoolSetting,
		.defaultValue = false, }},
};

using OldOptionKey = QString;
using NewOptionKey = QString;

const std::map<OldOptionKey, NewOptionKey, std::greater<OldOptionKey>> ReplacedOptionsMap {
	{ "adaptive_baloons", "adaptive_bubbles" },
};

QString DefaultFilePath() {
	return cWorkingDir() + qsl("tdata/FA-settings-default.json");
}

QString CustomFilePath() {
	return cWorkingDir() + qsl("tdata/FA-settings-custom.json");
}

bool DefaultFileIsValid() {
	QFile file(DefaultFilePath());
	if (!file.open(QIODevice::ReadOnly)) {
		return false;
	}
	auto error = QJsonParseError{ 0, QJsonParseError::NoError };
	const auto document = QJsonDocument::fromJson(
		base::parse::stripComments(file.readAll()),
		&error);
	file.close();

	if (error.error != QJsonParseError::NoError || !document.isObject()) {
		return false;
	}
	const auto settings = document.object();

	const auto version = settings.constFind(qsl("version"));
	if (version == settings.constEnd() || (*version).toInt() != AppFAVersion) {
		return false;
	}

	return true;
}

void WriteDefaultCustomFile() {
	const auto path = CustomFilePath();
	auto input = QFile(":/misc/default_FA-settings-custom.json");
	auto output = QFile(path);
	if (input.open(QIODevice::ReadOnly) && output.open(QIODevice::WriteOnly)) {
		output.write(input.readAll());
	}
}

QByteArray GenerateSettingsJson(bool areDefault = false) {
	auto settings = QJsonObject();

	auto settingsFoldersLocal = QJsonObject();

	const auto getRef = [&settings] (
			QStringList &keyParts,
			const Definition &def) -> QJsonValueRef {
		const auto firstKey = keyParts.takeFirst();
		if (!settings.contains(firstKey)) {
			settings.insert(firstKey, QJsonObject());
		}
		auto resultRef = settings[firstKey];
		for (const auto &key : keyParts) {
			auto referenced = resultRef.toObject();
			if (!referenced.contains(key)) {
				referenced.insert(key, QJsonObject());
				resultRef = referenced;
			}
			resultRef = referenced[key];
		}
		return resultRef;
	};

	const auto getValue = [=] (
			const QString &key,
			const Definition &def) -> QJsonValue {
		auto value = (!areDefault)
						? GetWithPending(key)
						: def.fillerValue.isValid()
						? def.fillerValue
						: def.defaultValue.isValid()
						? def.defaultValue
						: QVariant();
		switch (def.type) {
			case SettingType::BoolSetting:
				return value.isValid() ? value.toBool() : false;
			case SettingType::IntSetting:
				return value.isValid() ? value.toInt() : 0;
			case SettingType::QStringSetting:
				return value.isValid() ? value.toString() : QString();
			case SettingType::QJsonArraySetting:
				return value.isValid() ? value.toJsonArray() : QJsonArray();
		}

		return QJsonValue();
	};

	const auto getAccountValue = [=] (const QString &key) -> QJsonValue {
		if (areDefault) {
			return QJsonObject();
		}

		auto values = GetAllWithPending(key);
		auto resultObject = QJsonObject();

		for (auto i = values.constBegin(); i != values.constEnd(); ++i) {
			const auto value = i.value();
			const auto jsonValue = (value.userType() == QMetaType::Bool)
									? QJsonValue(value.toBool())
									: (value.userType() == QMetaType::Int)
									? QJsonValue(value.toInt())
									: (value.userType() == QMetaType::QString)
									? QJsonValue(value.toString())
									: (value.userType() == QMetaType::QJsonArray)
									? QJsonValue(value.toJsonArray())
									: QJsonValue(QJsonValue::Null);
			resultObject.insert(i.key(), jsonValue);
		}

		return resultObject;
	};

	for (const auto &[key, def] : DefinitionMap) {
		if (def.storage == SettingStorage::None) {
			continue;
		}

		auto parts = key.split(QChar('/'));
		auto value = (def.scope == SettingScope::Account)
						? getAccountValue(key)
						: getValue(key, def);
		if (parts.size() > 1) {
			const auto lastKey = parts.takeLast();
			auto ref = getRef(parts, def);
			auto referenced = ref.toObject();
			referenced.insert(lastKey, value);
			ref = referenced;
		} else {
			settings.insert(key, value);
		}
	}

	if (areDefault) {
		settings.insert(qsl("version"), QString::number(AppFAVersion));
	}

	auto document = QJsonDocument();
	document.setObject(settings);
	return document.toJson(QJsonDocument::Indented);
}

std::unique_ptr<Manager> Data;

QVariantMap GetAllWithPending(const QString &key) {
	return (Data) ? Data->getAllWithPending(key) : QVariantMap();
}

} // namespace

Manager::Manager()
: _jsonWriteTimer([=] { writeTimeout(); }) {
}

void Manager::load() {
	if (!DefaultFileIsValid()) {
		writeDefaultFile();
	}
	if (!readCustomFile()) {
		WriteDefaultCustomFile();
	}
}

void Manager::fill() {
	_settingsHashMap.reserve(DefinitionMap.size());
	_defaultSettingsHashMap.reserve(DefinitionMap.size());

	const auto addDefaultValue = [&] (const QString &option, QVariant value) {
		_settingsHashMap.insert(option, value);
	};

	for (const auto &[key, def] : DefinitionMap) {
		if (def.scope != SettingScope::Global) {
			continue;
		}

		auto defaultValue = def.defaultValue;
		if (!defaultValue.isValid()) {
			if (def.type == SettingType::BoolSetting) {
				defaultValue = false;
			} else if (def.type == SettingType::IntSetting) {
				defaultValue = 0;
			} else if (def.type == SettingType::QStringSetting) {
				defaultValue = QString();
			} else if (def.type == SettingType::QJsonArraySetting) {
				defaultValue = QJsonArray();
			} else {
				continue;
			}
		}

		addDefaultValue(key, defaultValue);
	}
}

void Manager::write(bool force) {
	if (force && _jsonWriteTimer.isActive()) {
		_jsonWriteTimer.cancel();
		writeTimeout();
	} else if (!force && !_jsonWriteTimer.isActive()) {
		_jsonWriteTimer.callOnce(kWriteJsonTimeout);
	}
}

QVariant Manager::get(const QString &key, uint64 accountId, bool isTestAccount) {
	const auto mapKey = MakeMapKey(key, accountId, isTestAccount);
	auto result = _settingsHashMap.contains(mapKey)
		? _settingsHashMap.value(mapKey)
		: QVariant();
	if (!result.isValid()) {
		result = _settingsHashMap.contains(key) 
					? _settingsHashMap.value(key)
					: getDefault(key);
		_settingsHashMap.insert(mapKey, result);
	}
	return result;
}

QVariant Manager::getWithPending(const QString &key, uint64 accountId, bool isTestAccount) {
	const auto mapKey = MakeMapKey(key, accountId, isTestAccount);
	auto result = _defaultSettingsHashMap.contains(mapKey)
		? _defaultSettingsHashMap.value(mapKey)
		: _settingsHashMap.contains(mapKey)
		? _settingsHashMap.value(mapKey)
		: QVariant();
	if (!result.isValid()) {
		result = _settingsHashMap.contains(key) 
					? _settingsHashMap.value(key)
					: getDefault(key);
		_settingsHashMap.insert(mapKey, result);
	}
	return result;
}

QVariantMap Manager::getAllWithPending(const QString &key) {
	auto resultMap = QVariantMap();

	if (_defaultSettingsHashMap.contains(key) || _settingsHashMap.contains(key)) {
		resultMap.insert(
			qsl("0"),
			_defaultSettingsHashMap.contains(key)
				? _defaultSettingsHashMap.value(key)
				: _settingsHashMap.value(key));
		return resultMap;
	}

	const auto prefix = key + qsl(":");

	for (auto i = _settingsHashMap.constBegin(); i != _settingsHashMap.constEnd(); ++i) {
		const auto mapKey = i.key();
		if (!mapKey.startsWith(prefix)) {
			continue;
		}

		const auto accountKey = mapKey.mid(prefix.size());
		resultMap.insert(accountKey, i.value());
	}

	for (auto i = _defaultSettingsHashMap.constBegin(); i != _defaultSettingsHashMap.constEnd(); ++i) {
		const auto mapKey = i.key();
		if (!mapKey.startsWith(prefix)) {
			continue;
		}

		const auto accountKey = mapKey.mid(prefix.size());
		resultMap.insert(accountKey, i.value());
	}

	return resultMap;
}

QVariant Manager::getDefault(const QString &key) {
	const auto &defIterator = DefinitionMap.find(key);
	if (defIterator == DefinitionMap.end()) {
		return QVariant();
	}
	const auto defaultValue = &defIterator->second.defaultValue;
	const auto settingType = defIterator->second.type;
	switch (settingType) {
		case SettingType::QStringSetting:
			return QVariant(defaultValue->isValid()
					? defaultValue->toString()
					: QString());
		case SettingType::IntSetting:
			return QVariant(defaultValue->isValid()
					? defaultValue->toInt()
					: 0);
		case SettingType::BoolSetting:
			return QVariant(defaultValue->isValid()
					? defaultValue->toBool()
					: false);
		case SettingType::QJsonArraySetting:
			return QVariant(defaultValue->isValid()
					? defaultValue->toJsonArray()
					: QJsonArray());
	}

	return QVariant();
}

rpl::producer<QString> Manager::events(const QString &key, uint64 accountId, bool isTestAccount) {
	const auto mapKey = MakeMapKey(key, accountId, isTestAccount);
	return _eventStream.events() | rpl::filter(rpl::mappers::_1 == mapKey);
}

rpl::producer<QString> Manager::eventsWithPending(const QString &key, uint64 accountId, bool isTestAccount) {
	const auto mapKey = MakeMapKey(key, accountId, isTestAccount);
	return _pendingEventStream.events() | rpl::filter(rpl::mappers::_1 == mapKey);
}

void Manager::set(const QString &key, QVariant value, uint64 accountId, bool isTestAccount) {
	const auto mapKey = MakeMapKey(key, accountId, isTestAccount);
	_settingsHashMap.insert(mapKey, value);
	_eventStream.fire_copy(mapKey);
}

void Manager::setAfterRestart(const QString &key, QVariant value, uint64 accountId, bool isTestAccount) {
	const auto mapKey = MakeMapKey(key, accountId, isTestAccount);
	if (!_settingsHashMap.contains(mapKey)
		|| _settingsHashMap.value(mapKey) != value) {
		_defaultSettingsHashMap.insert(mapKey, value);
	} else if (_settingsHashMap.contains(mapKey)
		&& _settingsHashMap.value(mapKey) == value) {
		_defaultSettingsHashMap.remove(mapKey);
	}
	_pendingEventStream.fire_copy(mapKey);
}

void Manager::reset(const QString &key, uint64 accountId, bool isTestAccount) {
	set(key, getDefault(key), accountId, isTestAccount);
}

void Manager::resetAfterRestart(const QString &key, uint64 accountId, bool isTestAccount) {
	setAfterRestart(key, getDefault(key), accountId, isTestAccount);
}

bool Manager::readCustomFile() {
	QFile file(CustomFilePath());
	if (!file.exists()) {
		return false;
	}
	if (!file.open(QIODevice::ReadOnly)) {
		return true;
	}
	auto error = QJsonParseError{ 0, QJsonParseError::NoError };
	const auto document = QJsonDocument::fromJson(
		base::parse::stripComments(file.readAll()),
		&error);
	file.close();

	if (error.error != QJsonParseError::NoError) {
		return true;
	} else if (!document.isObject()) {
		return true;
	}
	const auto settings = document.object();

	if (settings.isEmpty()) {
		return true;
	}

	const auto getObjectValue = [&settings] (
			QStringList &keyParts,
			const Definition &def) -> QJsonValue {
		const auto firstKey = keyParts.takeFirst();
		if (!settings.contains(firstKey)) {
			return QJsonValue();
		}
		auto resultRef = settings.value(firstKey);
		for (const auto &key : keyParts) {
			auto referenced = resultRef.toObject();
			if (!referenced.contains(key)) {
				return QJsonValue();
			}
			resultRef = referenced.value(key);
		}
		return resultRef;
	};

	const auto prepareAccountOptions = [] (
		const QString &key,
		const Definition &def,
		const QJsonValue &val,
		Fn<void(const QString &,
				const Definition &,
				const QJsonValue &,
				uint64,
				bool)> callback) {

		if (val.isUndefined()) {
			return;
		} else if (def.scope == SettingScope::Account && val.isObject()) {
			const auto accounts = val.toObject();
			if (accounts.isEmpty()) {
				return;
			}

			for (auto i = accounts.constBegin(); i != accounts.constEnd(); ++i) {
				auto optionKey = i.key();
				auto isTestAccount = false;
				if (optionKey.startsWith("test_")) {
					isTestAccount = true;
					optionKey = optionKey.mid(5);
				}
				auto accountId = optionKey.toULongLong();
				callback(key, def, i.value(), accountId, (accountId == 0) ? false : isTestAccount);
			}
		} else {
			callback(key, def, val, 0, false);
		}
	};

	const auto setValue = [this] (
		const QString &key,
		const Definition &def,
		const QJsonValue &val,
		uint64 accountId,
		bool isTestAccount) {

		const auto defType = def.type;
		if (defType == SettingType::BoolSetting) {
			if (val.isBool()) {
				set(key, val.toBool(), accountId, isTestAccount);
			} else if (val.isDouble()) {
				set(key, val.toDouble() != 0.0, accountId, isTestAccount);
			}
		} else if (defType == SettingType::IntSetting) {
			if (val.isDouble()) {
				auto intValue = qFloor(val.toDouble());
				set(key,
					(def.limitHandler)
						? def.limitHandler(intValue)
						: intValue,
					accountId,
					isTestAccount);
			}
		} else if (defType == SettingType::QStringSetting) {
			if (val.isString()) {
				set(key, val.toString(), accountId, isTestAccount);
			}
		} else if (defType == SettingType::QJsonArraySetting) {
			if (val.isArray()) {
				auto arrayValue = val.toArray();
				set(key, (def.limitHandler)
					? def.limitHandler(arrayValue)
					: arrayValue,
					accountId,
					isTestAccount);
			}
		}
	};

	for (const auto &[oldkey, newkey] : ReplacedOptionsMap) {
		const auto &defIterator = DefinitionMap.find(newkey);
		if (defIterator == DefinitionMap.end()) {
			continue;
		}
		auto parts = oldkey.split(QChar('/'));
		const auto val = (parts.size() > 1)
							? getObjectValue(parts, defIterator->second)
							: settings.value(oldkey);

		if (!val.isUndefined()) {
			prepareAccountOptions(newkey, defIterator->second, val, setValue);
		}
	}

	for (const auto &[key, def] : DefinitionMap) {
		if (def.storage == SettingStorage::None) {
			continue;
		}
		auto parts = key.split(QChar('/'));
		const auto val = (parts.size() > 1)
							? getObjectValue(parts, def)
							: settings.value(key);

		if (!val.isUndefined()) {
			prepareAccountOptions(key, def, val, setValue);
		}
	}
	return true;
}

void Manager::writeDefaultFile() {
	auto file = QFile(DefaultFilePath());
	if (!file.open(QIODevice::WriteOnly)) {
		return;
	}
	const char *defaultHeader = R"HEADER(
// This is a list of default options for FAGram Desktop
// Please don't modify it, its content is not used in any way
// You can place your own options in the 'FA-settings-custom.json' file

)HEADER";
	file.write(defaultHeader);
	file.write(GenerateSettingsJson(true));
}

void Manager::writeCurrentSettings() {
	auto file = QFile(CustomFilePath());
	if (!file.open(QIODevice::WriteOnly)) {
		return;
	}
	if (_jsonWriteTimer.isActive()) {
		writing();
	}
	const char *customHeader = R"HEADER(
// This file was automatically generated from current settings
// It's better to edit it with app closed, so there will be no rewrites
// You should restart app to see changes

)HEADER";
	file.write(customHeader);
	file.write(GenerateSettingsJson());
}

void Manager::writeTimeout() {
	writeCurrentSettings();
}

void Manager::writing() {
	_jsonWriteTimer.cancel();
}

void Start() {
	if (Data) return;

	Data = std::make_unique<Manager>();
	Data->fill();
}

void Load() {
	if (!Data) return;

	Data->load();
}

void Write() {
	if (!Data) return;

	Data->write();
}

void Finish() {
	if (!Data) return;

	Data->write(true);
}

QVariant Get(const QString &key, uint64 accountId, bool isTestAccount) {
	return (Data) ? Data->get(key, accountId, isTestAccount) : QVariant();
}

QVariant GetWithPending(const QString &key, uint64 accountId, bool isTestAccount) {
	return (Data) ? Data->getWithPending(key, accountId, isTestAccount) : QVariant();
}

rpl::producer<QString> Events(const QString &key, uint64 accountId, bool isTestAccount) {
	return (Data) ? Data->events(key, accountId, isTestAccount) : rpl::single(QString());
}

rpl::producer<QString> EventsWithPending(const QString &key, uint64 accountId, bool isTestAccount) {
	return (Data) ? Data->eventsWithPending(key, accountId, isTestAccount) : rpl::single(QString());
}

void Set(const QString &key, QVariant value, uint64 accountId, bool isTestAccount) {
	if (!Data) return;

	Data->set(key, value, accountId, isTestAccount);
}

void SetAfterRestart(const QString &key, QVariant value, uint64 accountId, bool isTestAccount) {
	if (!Data) return;

	Data->setAfterRestart(key, value, accountId, isTestAccount);
}

void Reset(const QString &key, uint64 accountId, bool isTestAccount) {
	if (!Data) return;

	Data->reset(key, accountId, isTestAccount);
}

void ResetAfterRestart(const QString &key, uint64 accountId, bool isTestAccount) {
	if (!Data) return;

	Data->resetAfterRestart(key, accountId, isTestAccount);
}

} // namespace JsonSettings
} // namespace FASettings