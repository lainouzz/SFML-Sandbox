#include "UI.h"
#include <SFML/Graphics.hpp>
#include "Constants.h"

void setupText(sf::Text& text, const sf::Font& font, const std::string& str, unsigned int size, sf::Vector2f pos, sf::Color color) {
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(size);
	text.setPosition(pos);
	text.setFillColor(color);
}

void renderUI(sf::RenderWindow& window, const sf::Font& font, float fps, bool overwriteMode) {
	sf::Text frameText(font);
	sf::Text instructions(font);
	sf::Text scrollText(font);
	sf::Text grainCountText(font);

	setupText(frameText, font, "FPS: " + std::to_string(static_cast<int>(fps)), 15, { 15.f, 15.f });
	setupText(instructions, font, "Left Click: Draw | Right Click: Erase | Overwrite Mode : " + std::string(overwriteMode ? "ON" : "OFF"), 15, { 15.f, 35.f });
	setupText(scrollText, font, "Scroll: Change Brush Size (" + std::to_string(brushRadius) + ")", 15, { 15.f, 55.f });
	setupText(grainCountText, font, "Grains: " + std::to_string(grainCount), 15, { 75.f, 15.f });

	window.draw(frameText);
	window.draw(instructions);
	window.draw(grainCountText);
	window.draw(scrollText);
}

void centerText(sf::Text& text, sf::Vector2f center, float y)
{
	sf::FloatRect bounds = text.getLocalBounds();

	text.setOrigin({
		bounds.position.x + bounds.size.x / 2.f,
		bounds.position.y + bounds.size.y / 2.f
	});
	text.setPosition({ center.x, y });
}