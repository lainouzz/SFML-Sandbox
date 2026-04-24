#pragma once

#include <SFML/Graphics.hpp>
#include "Cell.h"
#include <vector>

enum class GameState { MainMenu, Playing };

void handleEvents(sf::RenderWindow& window, sf::Color& selectedColor, MaterialType& selectedMaterial, bool& overwriteMode, std::vector<std::vector<Cell>>& grid, GameState& gameState);
void handleMouseInput(sf::RenderWindow& window, sf::Color selectedColor, MaterialType selectedMaterial, bool overwriteMode, std::vector<std::vector<Cell>>& grid);