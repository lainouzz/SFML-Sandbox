#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

#include "Constants.h"
#include "Cell.h"
#include "Grid.h"
#include "Physics.h"
#include "Input.h"
#include "Renderer.h"
#include "UI.h"
#include "MainMenu.h"

/*FUTURE IDEAS
- Fire + Temperature System
- Fluid Simulation Upgrade (pressure, flow, etc.)
- Explosions / Destruction
- save/load system
- Brush & UX Improvements
*/

int brushRadius = 1;
int grainCount = 0;

int main() {
	sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Sand Sandbox");
	window.setFramerateLimit(60);

	sf::Clock clock;
	std::srand(static_cast<unsigned>(time(0)));

	// ---------------- GRID ----------------
	std::vector<std::vector<Cell>> grid(GRID_HEIGHT, std::vector<Cell>(GRID_WIDTH));

	// ---------------- PIXEL/IMAGE ----------------
	sf::Image image(sf::Vector2u(GRID_WIDTH, GRID_HEIGHT), sf::Color::Transparent);
	sf::Texture texture(image);
	sf::Sprite sprite(texture);
	sprite.setScale({ (float)CELL_SIZE, (float)CELL_SIZE });

	sf::Color selectedColor(220, 185, 105);
	MaterialType selectedMaterial = MaterialType::Sand;
	bool overwriteMode = false;

	// ---------------- FONT ----------------
	sf::Font font("C:\\Users\\mojan\\source\\repos\\SMFLPhysics\\x64\\Debug\\font\\ARIAL.TTF");
	
	GameState gameState = GameState::MainMenu;

	while (window.isOpen()) {
		float deltaTime = clock.restart().asSeconds();
		float fps = (deltaTime > 0.f) ? 1.f / deltaTime : 0.f;

		if(gameState == GameState::MainMenu) {
			// ---------------- MENU EVENTS ----------------
			while (const std::optional event = window.pollEvent()) {
				if (event->is<sf::Event::Closed>())
					window.close();

			if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
				if (key->code == sf::Keyboard::Key::Enter) {
					gameState = GameState::Playing;
				}
				else if (key->code == sf::Keyboard::Key::Escape) {
					window.close();
				}
			}
			}
			// ---------------- MENU RENDER ----------------
			renderMainMenu(window, font);
		}
		else if (gameState == GameState::Playing) {
			// ---------------- EVENTS & INPUT ----------------
			handleEvents(window, selectedColor, selectedMaterial, overwriteMode, grid, gameState);
			handleMouseInput(window, selectedColor, selectedMaterial, overwriteMode, grid);

			// ---------------- PHYSICS ----------------
			updateSandPhysics(grid);
			updateFluidPhysics(grid);
			updateFirePhysics(grid);
			updateSmokePhysics(grid);

			// ---------------- RENDER ----------------
			window.clear(sf::Color(18, 18, 28));
			renderGrid(window, grid, image, texture, sprite);
			renderPalette(window, font, selectedColor);
			renderUI(window, font, fps, overwriteMode);
			window.display();
		}
		
	}

	return 0;
}
