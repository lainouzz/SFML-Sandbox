#include "Input.h"
#include "Constants.h"
#include "MainMenu.h"
#include "Grid.h"

void handleEvents(sf::RenderWindow& window, sf::Color& selectedColor, MaterialType& selectedMaterial, bool& overwriteMode, std::vector<std::vector<Cell>>& grid, GameState& gameState) {
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>())
			window.close();

		if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
			if (key->code == sf::Keyboard::Key::Num1) { selectedColor = MaterialColors::Sand;  selectedMaterial = MaterialType::Sand; }
			if (key->code == sf::Keyboard::Key::Num2) { selectedColor = MaterialColors::WetSand; selectedMaterial = MaterialType::WetSand; }
			if (key->code == sf::Keyboard::Key::Num3) { selectedColor = MaterialColors::Stone;   selectedMaterial = MaterialType::Stone; }
			if (key->code == sf::Keyboard::Key::Num4) { selectedColor = MaterialColors::Wood;    selectedMaterial = MaterialType::Wood; }
			if (key->code == sf::Keyboard::Key::Num5) { selectedColor = MaterialColors::Water;   selectedMaterial = MaterialType::Water; }
			if (key->code == sf::Keyboard::Key::Num6) { selectedColor = MaterialColors::Lava;    selectedMaterial = MaterialType::Lava; }
			if (key->code == sf::Keyboard::Key::Num7) { selectedColor = MaterialColors::Fire;    selectedMaterial = MaterialType::Fire; }

			if (key->code == sf::Keyboard::Key::C) { clearGrid(grid); }
			if (key->code == sf::Keyboard::Key::O) { overwriteMode = !overwriteMode; }
			if (key->code == sf::Keyboard::Key::Escape) { gameState = GameState::MainMenu; clearGrid(grid); }
		}

		if (const auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
			if (scroll->delta > 0)
				brushRadius = std::min(brushRadius + 1, 15);
			else if (scroll->delta < 0)
				brushRadius = std::max(brushRadius - 1, 1);
		}
	}
}

void handleMouseInput(sf::RenderWindow& window, sf::Color selectedColor, MaterialType selectedMaterial, bool overwriteMode, std::vector<std::vector<Cell>>& grid) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		int gx = pos.x / CELL_SIZE;
		int gy = pos.y / CELL_SIZE;

		forEachBrushCell(gx, gy, brushRadius, [&](int nx, int ny) {
			if(overwriteMode || !grid[ny][nx].filled) {
				grid[ny][nx].filled = true;
				grid[ny][nx].type = selectedMaterial;
				grid[ny][nx].color = varyColor(selectedColor);

				if (selectedMaterial == MaterialType::Water || selectedMaterial == MaterialType::Lava)
					grid[ny][nx].mass = 1.f;
				else if (selectedMaterial == MaterialType::Fire) {
					grid[ny][nx].mass = 8.f;
					grid[ny][nx].burning = true;
					grid[ny][nx].temperature = 300.f;
				}
				else if (selectedMaterial == MaterialType::Wood) {
					grid[ny][nx].mass = 10.f;
				}
				else
				{
					grid[ny][nx].mass = 0.f;
				}
				grainCount++;
			}
		});
	}

	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		int gx = pos.x / CELL_SIZE;
		int gy = pos.y / CELL_SIZE;

		forEachBrushCell(gx, gy, brushRadius, [&](int nx, int ny) {
			if(grid[ny][nx].filled) {
				grid[ny][nx] = Cell{};
				grainCount = std::max(0, grainCount - 1);
			}
		});
	}
}