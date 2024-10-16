/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#include "data/raw/raw_countries_bounds.h"

// Source: https://github.com/sandstrom/country-bounding-boxes

namespace Raw {

const base::flat_map<QString, GeoBounds> &CountryBounds() {
	static const auto result = base::flat_map<QString, GeoBounds>{
		{ u"AF"_q, GeoBounds{ 60.53, 29.32, 75.16, 38.49 } },
		{ u"AO"_q, GeoBounds{ 11.64, -17.93, 24.08, -4.44 } },
		{ u"AL"_q, GeoBounds{ 19.3, 39.62, 21.02, 42.69 } },
		{ u"AE"_q, GeoBounds{ 51.58, 22.5, 56.4, 26.06 } },
		{ u"AR"_q, GeoBounds{ -73.42, -55.25, -53.63, -21.83 } },
		{ u"AM"_q, GeoBounds{ 43.58, 38.74, 46.51, 41.25 } },
		{ u"AQ"_q, GeoBounds{ -180.0, -90.0, 180.0, -63.27 } },
		{ u"TF"_q, GeoBounds{ 68.72, -49.78, 70.56, -48.63 } },
		{ u"AU"_q, GeoBounds{ 113.34, -43.63, 153.57, -10.67 } },
		{ u"AT"_q, GeoBounds{ 9.48, 46.43, 16.98, 49.04 } },
		{ u"AZ"_q, GeoBounds{ 44.79, 38.27, 50.39, 41.86 } },
		{ u"BI"_q, GeoBounds{ 29.02, -4.5, 30.75, -2.35 } },
		{ u"BE"_q, GeoBounds{ 2.51, 49.53, 6.16, 51.48 } },
		{ u"BJ"_q, GeoBounds{ 0.77, 6.14, 3.8, 12.24 } },
		{ u"BF"_q, GeoBounds{ -5.47, 9.61, 2.18, 15.12 } },
		{ u"BD"_q, GeoBounds{ 88.08, 20.67, 92.67, 26.45 } },
		{ u"BG"_q, GeoBounds{ 22.38, 41.23, 28.56, 44.23 } },
		{ u"BS"_q, GeoBounds{ -78.98, 23.71, -77.0, 27.04 } },
		{ u"BA"_q, GeoBounds{ 15.75, 42.65, 19.6, 45.23 } },
		{ u"BY"_q, GeoBounds{ 23.2, 51.32, 32.69, 56.17 } },
		{ u"BZ"_q, GeoBounds{ -89.23, 15.89, -88.11, 18.5 } },
		{ u"BO"_q, GeoBounds{ -69.59, -22.87, -57.5, -9.76 } },
		{ u"BR"_q, GeoBounds{ -73.99, -33.77, -34.73, 5.24 } },
		{ u"BN"_q, GeoBounds{ 114.2, 4.01, 115.45, 5.45 } },
		{ u"BT"_q, GeoBounds{ 88.81, 26.72, 92.1, 28.3 } },
		{ u"BW"_q, GeoBounds{ 19.9, -26.83, 29.43, -17.66 } },
		{ u"CF"_q, GeoBounds{ 14.46, 2.27, 27.37, 11.14 } },
		{ u"CA"_q, GeoBounds{ -141.0, 41.68, -52.65, 73.23 } },
		{ u"CH"_q, GeoBounds{ 6.02, 45.78, 10.44, 47.83 } },
		{ u"CL"_q, GeoBounds{ -75.64, -55.61, -66.96, -17.58 } },
		{ u"CN"_q, GeoBounds{ 73.68, 18.2, 135.03, 53.46 } },
		{ u"CI"_q, GeoBounds{ -8.6, 4.34, -2.56, 10.52 } },
		{ u"CM"_q, GeoBounds{ 8.49, 1.73, 16.01, 12.86 } },
		{ u"CD"_q, GeoBounds{ 12.18, -13.26, 31.17, 5.26 } },
		{ u"CG"_q, GeoBounds{ 11.09, -5.04, 18.45, 3.73 } },
		{ u"CO"_q, GeoBounds{ -78.99, -4.3, -66.88, 12.44 } },
		{ u"CR"_q, GeoBounds{ -85.94, 8.23, -82.55, 11.22 } },
		{ u"CU"_q, GeoBounds{ -84.97, 19.86, -74.18, 23.19 } },
		{ u"CY"_q, GeoBounds{ 32.26, 34.57, 34.0, 35.17 } },
		{ u"CZ"_q, GeoBounds{ 12.24, 48.56, 18.85, 51.12 } },
		{ u"DE"_q, GeoBounds{ 5.99, 47.3, 15.02, 54.98 } },
		{ u"DJ"_q, GeoBounds{ 41.66, 10.93, 43.32, 12.7 } },
		{ u"DK"_q, GeoBounds{ 8.09, 54.8, 12.69, 57.73 } },
		{ u"DO"_q, GeoBounds{ -71.95, 17.6, -68.32, 19.88 } },
		{ u"DZ"_q, GeoBounds{ -8.68, 19.06, 12.0, 37.12 } },
		{ u"EC"_q, GeoBounds{ -80.97, -4.96, -75.23, 1.38 } },
		{ u"EG"_q, GeoBounds{ 24.7, 22.0, 36.87, 31.59 } },
		{ u"ER"_q, GeoBounds{ 36.32, 12.46, 43.08, 18.0 } },
		{ u"ES"_q, GeoBounds{ -9.39, 35.95, 3.04, 43.75 } },
		{ u"EE"_q, GeoBounds{ 23.34, 57.47, 28.13, 59.61 } },
		{ u"ET"_q, GeoBounds{ 32.95, 3.42, 47.79, 14.96 } },
		{ u"FI"_q, GeoBounds{ 20.65, 59.85, 31.52, 70.16 } },
		{ u"FJ"_q, GeoBounds{ -180.0, -18.29, 180.0, -16.02 } },
		{ u"FK"_q, GeoBounds{ -61.2, -52.3, -57.75, -51.1 } },
		{ u"FR"_q, GeoBounds{ -5.0, 42.5, 9.56, 51.15 } },
		{ u"GA"_q, GeoBounds{ 8.8, -3.98, 14.43, 2.33 } },
		{ u"GB"_q, GeoBounds{ -7.57, 49.96, 1.68, 58.64 } },
		{ u"GE"_q, GeoBounds{ 39.96, 41.06, 46.64, 43.55 } },
		{ u"GH"_q, GeoBounds{ -3.24, 4.71, 1.06, 11.1 } },
		{ u"GN"_q, GeoBounds{ -15.13, 7.31, -7.83, 12.59 } },
		{ u"GM"_q, GeoBounds{ -16.84, 13.13, -13.84, 13.88 } },
		{ u"GW"_q, GeoBounds{ -16.68, 11.04, -13.7, 12.63 } },
		{ u"GQ"_q, GeoBounds{ 9.31, 1.01, 11.29, 2.28 } },
		{ u"GR"_q, GeoBounds{ 20.15, 34.92, 26.6, 41.83 } },
		{ u"GL"_q, GeoBounds{ -73.3, 60.04, -12.21, 83.65 } },
		{ u"GT"_q, GeoBounds{ -92.23, 13.74, -88.23, 17.82 } },
		{ u"GY"_q, GeoBounds{ -61.41, 1.27, -56.54, 8.37 } },
		{ u"HN"_q, GeoBounds{ -89.35, 12.98, -83.15, 16.01 } },
		{ u"HR"_q, GeoBounds{ 13.66, 42.48, 19.39, 46.5 } },
		{ u"HT"_q, GeoBounds{ -74.46, 18.03, -71.62, 19.92 } },
		{ u"HU"_q, GeoBounds{ 16.2, 45.76, 22.71, 48.62 } },
		{ u"ID"_q, GeoBounds{ 95.29, -10.36, 141.03, 5.48 } },
		{ u"IN"_q, GeoBounds{ 68.18, 7.97, 97.4, 35.49 } },
		{ u"IE"_q, GeoBounds{ -9.98, 51.67, -6.03, 55.13 } },
		{ u"IR"_q, GeoBounds{ 44.11, 25.08, 63.32, 39.71 } },
		{ u"IQ"_q, GeoBounds{ 38.79, 29.1, 48.57, 37.39 } },
		{ u"IS"_q, GeoBounds{ -24.33, 63.5, -13.61, 66.53 } },
		{ u"IL"_q, GeoBounds{ 34.27, 29.5, 35.84, 33.28 } },
		{ u"IT"_q, GeoBounds{ 6.75, 36.62, 18.48, 47.12 } },
		{ u"JM"_q, GeoBounds{ -78.34, 17.7, -76.2, 18.52 } },
		{ u"JO"_q, GeoBounds{ 34.92, 29.2, 39.2, 33.38 } },
		{ u"JP"_q, GeoBounds{ 129.41, 31.03, 145.54, 45.55 } },
		{ u"KZ"_q, GeoBounds{ 46.47, 40.66, 87.36, 55.39 } },
		{ u"KE"_q, GeoBounds{ 33.89, -4.68, 41.86, 5.51 } },
		{ u"KG"_q, GeoBounds{ 69.46, 39.28, 80.26, 43.3 } },
		{ u"KH"_q, GeoBounds{ 102.35, 10.49, 107.61, 14.57 } },
		{ u"KR"_q, GeoBounds{ 126.12, 34.39, 129.47, 38.61 } },
		{ u"KW"_q, GeoBounds{ 46.57, 28.53, 48.42, 30.06 } },
		{ u"LA"_q, GeoBounds{ 100.12, 13.88, 107.56, 22.46 } },
		{ u"LB"_q, GeoBounds{ 35.13, 33.09, 36.61, 34.64 } },
		{ u"LR"_q, GeoBounds{ -11.44, 4.36, -7.54, 8.54 } },
		{ u"LY"_q, GeoBounds{ 9.32, 19.58, 25.16, 33.14 } },
		{ u"LK"_q, GeoBounds{ 79.7, 5.97, 81.79, 9.82 } },
		{ u"LS"_q, GeoBounds{ 27.0, -30.65, 29.33, -28.65 } },
		{ u"LT"_q, GeoBounds{ 21.06, 53.91, 26.59, 56.37 } },
		{ u"LU"_q, GeoBounds{ 5.67, 49.44, 6.24, 50.13 } },
		{ u"LV"_q, GeoBounds{ 21.06, 55.62, 28.18, 57.97 } },
		{ u"MA"_q, GeoBounds{ -17.02, 21.42, -1.12, 35.76 } },
		{ u"MD"_q, GeoBounds{ 26.62, 45.49, 30.02, 48.47 } },
		{ u"MG"_q, GeoBounds{ 43.25, -25.6, 50.48, -12.04 } },
		{ u"MX"_q, GeoBounds{ -117.13, 14.54, -86.81, 32.72 } },
		{ u"MK"_q, GeoBounds{ 20.46, 40.84, 22.95, 42.32 } },
		{ u"ML"_q, GeoBounds{ -12.17, 10.1, 4.27, 24.97 } },
		{ u"MM"_q, GeoBounds{ 92.3, 9.93, 101.18, 28.34 } },
		{ u"ME"_q, GeoBounds{ 18.45, 41.88, 20.34, 43.52 } },
		{ u"MN"_q, GeoBounds{ 87.75, 41.6, 119.77, 52.05 } },
		{ u"MZ"_q, GeoBounds{ 30.18, -26.74, 40.78, -10.32 } },
		{ u"MR"_q, GeoBounds{ -17.06, 14.62, -4.92, 27.4 } },
		{ u"MW"_q, GeoBounds{ 32.69, -16.8, 35.77, -9.23 } },
		{ u"MY"_q, GeoBounds{ 100.09, 0.77, 119.18, 6.93 } },
		{ u"NA"_q, GeoBounds{ 11.73, -29.05, 25.08, -16.94 } },
		{ u"NC"_q, GeoBounds{ 164.03, -22.4, 167.12, -20.11 } },
		{ u"NE"_q, GeoBounds{ 0.3, 11.66, 15.9, 23.47 } },
		{ u"NG"_q, GeoBounds{ 2.69, 4.24, 14.58, 13.87 } },
		{ u"NI"_q, GeoBounds{ -87.67, 10.73, -83.15, 15.02 } },
		{ u"NL"_q, GeoBounds{ 3.31, 50.8, 7.09, 53.51 } },
		{ u"NO"_q, GeoBounds{ 4.99, 58.08, 31.29, 70.92 } },
		{ u"NP"_q, GeoBounds{ 80.09, 26.4, 88.17, 30.42 } },
		{ u"NZ"_q, GeoBounds{ 166.51, -46.64, 178.52, -34.45 } },
		{ u"OM"_q, GeoBounds{ 52.0, 16.65, 59.81, 26.4 } },
		{ u"PK"_q, GeoBounds{ 60.87, 23.69, 77.84, 37.13 } },
		{ u"PA"_q, GeoBounds{ -82.97, 7.22, -77.24, 9.61 } },
		{ u"PE"_q, GeoBounds{ -81.41, -18.35, -68.67, -0.06 } },
		{ u"PH"_q, GeoBounds{ 117.17, 5.58, 126.54, 18.51 } },
		{ u"PG"_q, GeoBounds{ 141.0, -10.65, 156.02, -2.5 } },
		{ u"PL"_q, GeoBounds{ 14.07, 49.03, 24.03, 54.85 } },
		{ u"PR"_q, GeoBounds{ -67.24, 17.95, -65.59, 18.52 } },
		{ u"KP"_q, GeoBounds{ 124.27, 37.67, 130.78, 42.99 } },
		{ u"PT"_q, GeoBounds{ -9.53, 36.84, -6.39, 42.28 } },
		{ u"PY"_q, GeoBounds{ -62.69, -27.55, -54.29, -19.34 } },
		{ u"QA"_q, GeoBounds{ 50.74, 24.56, 51.61, 26.11 } },
		{ u"RO"_q, GeoBounds{ 20.22, 43.69, 29.63, 48.22 } },
		{ u"RU"_q, GeoBounds{ -180.0, 41.15, 180.0, 81.25 } },
		{ u"RW"_q, GeoBounds{ 29.02, -2.92, 30.82, -1.13 } },
		{ u"SA"_q, GeoBounds{ 34.63, 16.35, 55.67, 32.16 } },
		{ u"SD"_q, GeoBounds{ 21.94, 8.62, 38.41, 22.0 } },
		{ u"SS"_q, GeoBounds{ 23.89, 3.51, 35.3, 12.25 } },
		{ u"SN"_q, GeoBounds{ -17.63, 12.33, -11.47, 16.6 } },
		{ u"SB"_q, GeoBounds{ 156.49, -10.83, 162.4, -6.6 } },
		{ u"SL"_q, GeoBounds{ -13.25, 6.79, -10.23, 10.05 } },
		{ u"SV"_q, GeoBounds{ -90.1, 13.15, -87.72, 14.42 } },
		{ u"SO"_q, GeoBounds{ 40.98, -1.68, 51.13, 12.02 } },
		{ u"RS"_q, GeoBounds{ 18.83, 42.25, 22.99, 46.17 } },
		{ u"SR"_q, GeoBounds{ -58.04, 1.82, -53.96, 6.03 } },
		{ u"SK"_q, GeoBounds{ 16.88, 47.76, 22.56, 49.57 } },
		{ u"SI"_q, GeoBounds{ 13.7, 45.45, 16.56, 46.85 } },
		{ u"SE"_q, GeoBounds{ 11.03, 55.36, 23.9, 69.11 } },
		{ u"SZ"_q, GeoBounds{ 30.68, -27.29, 32.07, -25.66 } },
		{ u"SY"_q, GeoBounds{ 35.7, 32.31, 42.35, 37.23 } },
		{ u"TD"_q, GeoBounds{ 13.54, 7.42, 23.89, 23.41 } },
		{ u"TG"_q, GeoBounds{ -0.05, 5.93, 1.87, 11.02 } },
		{ u"TH"_q, GeoBounds{ 97.38, 5.69, 105.59, 20.42 } },
		{ u"TJ"_q, GeoBounds{ 67.44, 36.74, 74.98, 40.96 } },
		{ u"TM"_q, GeoBounds{ 52.5, 35.27, 66.55, 42.75 } },
		{ u"TL"_q, GeoBounds{ 124.97, -9.39, 127.34, -8.27 } },
		{ u"TT"_q, GeoBounds{ -61.95, 10.0, -60.9, 10.89 } },
		{ u"TN"_q, GeoBounds{ 7.52, 30.31, 11.49, 37.35 } },
		{ u"TR"_q, GeoBounds{ 26.04, 35.82, 44.79, 42.14 } },
		{ u"TW"_q, GeoBounds{ 120.11, 21.97, 121.95, 25.3 } },
		{ u"TZ"_q, GeoBounds{ 29.34, -11.72, 40.32, -0.95 } },
		{ u"UG"_q, GeoBounds{ 29.58, -1.44, 35.04, 4.25 } },
		{ u"UA"_q, GeoBounds{ 22.09, 44.36, 40.08, 52.34 } },
		{ u"UY"_q, GeoBounds{ -58.43, -34.95, -53.21, -30.11 } },
		{ u"US"_q, GeoBounds{ -125.0, 25.0, -66.96, 49.5 } },
		{ u"UZ"_q, GeoBounds{ 55.93, 37.14, 73.06, 45.59 } },
		{ u"VE"_q, GeoBounds{ -73.3, 0.72, -59.76, 12.16 } },
		{ u"VN"_q, GeoBounds{ 102.17, 8.6, 109.34, 23.35 } },
		{ u"VU"_q, GeoBounds{ 166.63, -16.6, 167.84, -14.63 } },
		{ u"PS"_q, GeoBounds{ 34.93, 31.35, 35.55, 32.53 } },
		{ u"YE"_q, GeoBounds{ 42.6, 12.59, 53.11, 19.0 } },
		{ u"ZA"_q, GeoBounds{ 16.34, -34.82, 32.83, -22.09 } },
		{ u"ZM"_q, GeoBounds{ 21.89, -17.96, 33.49, -8.24 } },
		{ u"ZW"_q, GeoBounds{ 25.26, -22.27, 32.85, -15.51 } }
	};
	return result;
}

} // namespace Raw
