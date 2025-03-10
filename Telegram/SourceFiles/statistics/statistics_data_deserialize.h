/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#pragma once

namespace Data {
struct StatisticalChart;
} // namespace Data

class QByteArray;

namespace Statistic {

[[nodiscard]] Data::StatisticalChart StatisticalChartFromJSON(
	const QByteArray &json);

} // namespace Statistic
