#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "Constants.h"

enum class MaterialType { Empty, Sand, WetSand, Stone, Wood, Water, Lava, Fire, Smoke };

namespace MaterialColors{
	inline const sf::Color Sand(220, 185, 105);
	inline const sf::Color WetSand(196, 163, 90);
	inline const sf::Color Stone(128, 128, 128);
	inline const sf::Color Wood(160, 82, 45);
	inline const sf::Color Water(50, 114, 208);
	inline const sf::Color Lava(232, 69, 32);
	inline const sf::Color Fire(255, 69, 0);
	inline const sf::Color Smoke(105, 105, 105);
}

namespace UIColors {
	inline const sf::Color Background(18, 18, 28);
	inline const sf::Color Text(255, 255, 255);
	inline const sf::Color Highlight(80, 80, 80);
}

struct Cell {
	bool filled = false;
	bool burning = false;

	float mass = 0.f;
	float temperature = 20.f;
	float velocityX = 0.f;
	float velocityY = 0.f;

	MaterialType type = MaterialType::Empty;
	sf::Color color = sf::Color::Transparent;
};

struct Palett {
	sf::Color color;
	std::string label;

	int key;
};

inline const std::vector<Palett> palette = {
	{ sf::Color(220, 185, 105), "Sand",  1 },
	{ sf::Color(196, 163,  90), "WSand", 2 },
	{ sf::Color(128, 128, 128), "Stone", 3 },
	{ sf::Color(160,  82,  45), "Wood",  4 },
	{ sf::Color(50, 114, 208), "Water", 5 },
	{ sf::Color(232,  69,  32), "Lava",  6 },
	{ sf::Color(255,  69,   0), "Fire",  7 },
};

inline bool inBounds(int x, int y) {
	return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
}

inline sf::Color varyColor(sf::Color base, int range = 15) {
	auto clamp = [](int v) { return std::max(0, std::min(255, v)); };
	int r = (rand() % (range * 2 + 1)) - range;
	return sf::Color(
		clamp(base.r + r),
		clamp(base.g + r),
		clamp(base.b + r)
	);
}

template<typename F>
void forEachBrushCell(int cx, int cy, int radius, F action) {
	for (int dy = -radius; dy <= radius; dy++) {
		for (int dx = -radius; dx <= radius; dx++) {
			if (dx * dx + dy * dy > radius * radius)
				continue;
			int nx = cx + dx;
			int ny = cy + dy;
			if(inBounds(nx, ny))
				action(nx, ny);
		}
	}
}

