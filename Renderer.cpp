#include "Renderer.h"
#include "Constants.h"

void renderGrid(sf::RenderWindow& window, const std::vector<std::vector<Cell>>& grid, sf::Image& image, sf::Texture& texture, sf::Sprite& sprite) {
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			if (grid[y][x].filled)
				image.setPixel(sf::Vector2u(x, y), grid[y][x].color);
			else
				image.setPixel(sf::Vector2u(x, y), UIColors::Background);
		}
	}

	texture.update(image);
	window.draw(sprite);
}

void renderPalette(sf::RenderWindow& window, const sf::Font& font, sf::Color selectedColor) {

	sf::RectangleShape box(sf::Vector2f(boxSize, boxSize));
	sf::Text paletteText(font);
	paletteText.setCharacterSize(10);

	for (int i = 0; i < (int)palette.size(); i++) {
		float x = startX + i * (boxSize + padding);

		if (palette[i].color == selectedColor) {
			box.setOutlineThickness(2.f);
			box.setOutlineColor(sf::Color::White);
		}
		else {
			box.setOutlineThickness(1.f);
			box.setOutlineColor(UIColors::Highlight);
		}

		box.setFillColor(palette[i].color);
		box.setPosition({ x, startY });
		window.draw(box);

		paletteText.setString(std::to_string(palette[i].key) + ":" + palette[i].label);
		paletteText.setPosition({ x, startY - 15.f });
		paletteText.setFillColor(sf::Color::White);
		window.draw(paletteText);
	}
}