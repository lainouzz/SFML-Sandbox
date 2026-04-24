#pragma once

#include <SFML/Graphics.hpp>
#include "Cell.h"
#include "Constants.h"
#include <vector>

inline constexpr float boxSize = 30.f;
inline constexpr float padding = 15.f;
inline const float startX = WINDOW_WIDTH - (boxSize + padding + 5.f) * palette.size();
inline const float startY = WINDOW_HEIGHT - boxSize - 525.f;

void renderGrid(sf::RenderWindow& window, const std::vector<std::vector<Cell>>& grid, sf::Image& image, sf::Texture& texture, sf::Sprite& sprite);
void renderPalette(sf::RenderWindow& window, const sf::Font& font, sf::Color selectedColor);