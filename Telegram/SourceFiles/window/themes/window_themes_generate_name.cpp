/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#include "window/themes/window_themes_generate_name.h"

#include "base/random.h"

namespace Window {
namespace Theme {
namespace {

const auto kColors = base::flat_map<uint32, const char*>{
	{ 0x8e0000U, "Berry" },
	{ 0xdec196U, "Brandy" },
	{ 0x800b47U, "Cherry" },
	{ 0xff7f50U, "Coral" },
	{ 0xdb5079U, "Cranberry" },
	{ 0xdc143cU, "Crimson" },
	{ 0xe0b0ffU, "Mauve" },
	{ 0xffc0cbU, "Pink" },
	{ 0xff0000U, "Red" },
	{ 0xff007fU, "Rose" },
	{ 0x80461bU, "Russet" },
	{ 0xff2400U, "Scarlet" },
	{ 0xf1f1f1U, "Seashell" },
	{ 0xff3399U, "Strawberry" },
	{ 0xffbf00U, "Amber" },
	{ 0xeb9373U, "Apricot" },
	{ 0xfbe7b2U, "Banana" },
	{ 0xa1c50aU, "Citrus" },
	{ 0xb06500U, "Ginger" },
	{ 0xffd700U, "Gold" },
	{ 0xfde910U, "Lemon" },
	{ 0xffa500U, "Orange" },
	{ 0xffe5b4U, "Peach" },
	{ 0xff6b53U, "Persimmon" },
	{ 0xe4d422U, "Sunflower" },
	{ 0xf28500U, "Tangerine" },
	{ 0xffc87cU, "Topaz" },
	{ 0xffff00U, "Yellow" },
	{ 0x384910U, "Clover" },
	{ 0x83aa5dU, "Cucumber" },
	{ 0x50c878U, "Emerald" },
	{ 0xb5b35cU, "Olive" },
	{ 0x00ff00U, "Green" },
	{ 0x00a86bU, "Jade" },
	{ 0x29ab87U, "Jungle" },
	{ 0xbfff00U, "Lime" },
	{ 0x0bda51U, "Malachite" },
	{ 0x98ff98U, "Mint" },
	{ 0xaddfadU, "Moss" },
	{ 0x315ba1U, "Azure" },
	{ 0x0000ffU, "Blue" },
	{ 0x0047abU, "Cobalt" },
	{ 0x4f69c6U, "Indigo" },
	{ 0x017987U, "Lagoon" },
	{ 0x71d9e2U, "Aquamarine" },
	{ 0x120a8fU, "Ultramarine" },
	{ 0x000080U, "Navy" },
	{ 0x2f519eU, "Sapphire" },
	{ 0x76d7eaU, "Sky" },
	{ 0x008080U, "Teal" },
	{ 0x40e0d0U, "Turquoise" },
	{ 0x9966ccU, "Amethyst" },
	{ 0x4d0135U, "Blackberry" },
	{ 0x614051U, "Eggplant" },
	{ 0xc8a2c8U, "Lilac" },
	{ 0xb57edcU, "Lavender" },
	{ 0xccccffU, "Periwinkle" },
	{ 0x843179U, "Plum" },
	{ 0x660099U, "Purple" },
	{ 0xd8bfd8U, "Thistle" },
	{ 0xda70d6U, "Orchid" },
	{ 0x240a40U, "Violet" },
	{ 0x3f2109U, "Bronze" },
	{ 0x370202U, "Chocolate" },
	{ 0x7b3f00U, "Cinnamon" },
	{ 0x301f1eU, "Cocoa" },
	{ 0x706555U, "Coffee" },
	{ 0x796989U, "Rum" },
	{ 0x4e0606U, "Mahogany" },
	{ 0x782d19U, "Mocha" },
	{ 0xc2b280U, "Sand" },
	{ 0x882d17U, "Sienna" },
	{ 0x780109U, "Maple" },
	{ 0xf0e68cU, "Khaki" },
	{ 0xb87333U, "Copper" },
	{ 0xb94e48U, "Chestnut" },
	{ 0xeed9c4U, "Almond" },
	{ 0xfffdd0U, "Cream" },
	{ 0xb9f2ffU, "Diamond" },
	{ 0xa98307U, "Honey" },
	{ 0xfffff0U, "Ivory" },
	{ 0xeae0c8U, "Pearl" },
	{ 0xeff2f3U, "Porcelain" },
	{ 0xd1bea8U, "Vanilla" },
	{ 0xffffffU, "White" },
	{ 0x808080U, "Gray" },
	{ 0x000000U, "Black" },
	{ 0xe8f1d4U, "Chrome" },
	{ 0x36454fU, "Charcoal" },
	{ 0x0c0b1dU, "Ebony" },
	{ 0xc0c0c0U, "Silver" },
	{ 0xf5f5f5U, "Smoke" },
	{ 0x262335U, "Steel" },
	{ 0x4fa83dU, "Apple" },
	{ 0x80b3c4U, "Glacier" },
	{ 0xfebaadU, "Melon" },
	{ 0xc54b8cU, "Mulberry" },
	{ 0xa9c6c2U, "Opal" },
	{ 0x54a5f8U, "Blue" }
};

const auto kAdjectives = std::vector<const char*>{
	"Ancient",
	"Antique",
	"Autumn",
	"Baby",
	"Barely",
	"Baroque",
	"Blazing",
	"Blushing",
	"Bohemian",
	"Bubbly",
	"Burning",
	"Buttered",
	"Classic",
	"Clear",
	"Cool",
	"Cosmic",
	"Cotton",
	"Cozy",
	"Crystal",
	"Dark",
	"Daring",
	"Darling",
	"Dawn",
	"Dazzling",
	"Deep",
	"Deepest",
	"Delicate",
	"Delightful",
	"Divine",
	"Double",
	"Downtown",
	"Dreamy",
	"Dusky",
	"Dusty",
	"Electric",
	"Enchanted",
	"Endless",
	"Evening",
	"Fantastic",
	"Flirty",
	"Forever",
	"Frigid",
	"Frosty",
	"Frozen",
	"Gentle",
	"Heavenly",
	"Hyper",
	"Icy",
	"Infinite",
	"Innocent",
	"Instant",
	"Luscious",
	"Lunar",
	"Lustrous",
	"Magic",
	"Majestic",
	"Mambo",
	"Midnight",
	"Millennium",
	"Morning",
	"Mystic",
	"Natural",
	"Neon",
	"Night",
	"Opaque",
	"Paradise",
	"Perfect",
	"Perky",
	"Polished",
	"Powerful",
	"Rich",
	"Royal",
	"Sheer",
	"Simply",
	"Sizzling",
	"Solar",
	"Sparkling",
	"Splendid",
	"Spicy",
	"Spring",
	"Stellar",
	"Sugared",
	"Summer",
	"Sunny",
	"Super",
	"Sweet",
	"Tender",
	"Tenacious",
	"Tidal",
	"Toasted",
	"Totally",
	"Tranquil",
	"Tropical",
	"True",
	"Twilight",
	"Twinkling",
	"Ultimate",
	"Ultra",
	"Velvety",
	"Vibrant",
	"Vintage",
	"Virtual",
	"Warm",
	"Warmest",
	"Whipped",
	"Wild",
	"Winsome"
};

const auto kSubjectives = std::vector<const char*>{
	"Ambrosia",
	"Attack",
	"Avalanche",
	"Blast",
	"Bliss",
	"Blossom",
	"Blush",
	"Burst",
	"Butter",
	"Candy",
	"Carnival",
	"Charm",
	"Chiffon",
	"Cloud",
	"Comet",
	"Delight",
	"Dream",
	"Dust",
	"Fantasy",
	"Flame",
	"Flash",
	"Fire",
	"Freeze",
	"Frost",
	"Glade",
	"Glaze",
	"Gleam",
	"Glimmer",
	"Glitter",
	"Glow",
	"Grande",
	"Haze",
	"Highlight",
	"Ice",
	"Illusion",
	"Intrigue",
	"Jewel",
	"Jubilee",
	"Kiss",
	"Lights",
	"Lollypop",
	"Love",
	"Luster",
	"Madness",
	"Matte",
	"Mirage",
	"Mist",
	"Moon",
	"Muse",
	"Myth",
	"Nectar",
	"Nova",
	"Parfait",
	"Passion",
	"Pop",
	"Rain",
	"Reflection",
	"Rhapsody",
	"Romance",
	"Satin",
	"Sensation",
	"Silk",
	"Shine",
	"Shadow",
	"Shimmer",
	"Sky",
	"Spice",
	"Star",
	"Sugar",
	"Sunrise",
	"Sunset",
	"Sun",
	"Twist",
	"Unbound",
	"Velvet",
	"Vibrant",
	"Waters",
	"Wine",
	"Wink",
	"Wonder",
	"Zone"
};

} // namespace

QString GenerateName(const QColor &accent) {
	const auto r1 = accent.red();
	const auto g1 = accent.green();
	const auto b1 = accent.blue();
	const auto distance = [&](const auto &pair) {
		const auto &[color, name] = pair;
		const auto b2 = int(color & 0xFFU);
		const auto g2 = int((color >> 8) & 0xFFU);
		const auto r2 = int((color >> 16) & 0xFFU);
		const auto rMean = (r1 + r2) / 2;
		const auto r = r1 - r2;
		const auto g = g1 - g2;
		const auto b = b1 - b2;
		return (((512 + rMean) * r * r) >> 8)
			+ (4 * g * g)
			+ (((767 - rMean) * b * b) >> 8);
	};
	const auto pred = [&](const auto &a, const auto &b) {
		return distance(a) < distance(b);
	};
	const auto capitalized = [](const char *value) {
		Expects(*value != 0);

		auto result = QString::fromLatin1(value);
		result[0] = result[0].toUpper();
		return result;
	};
	const auto random = [&](const std::vector<const char*> &values) {
		const auto index = base::RandomValue<size_t>() % values.size();
		return capitalized(values[index]);
	};
	const auto min = ranges::min_element(kColors, pred);
	Assert(min != end(kColors));
	const auto color = capitalized(min->second);
	return (base::RandomValue<uint8>() % 2 == 0)
		? random(kAdjectives) + ' ' + color
		: color + ' ' + random(kSubjectives);
}

} // namespace Theme
} // namespace Window
