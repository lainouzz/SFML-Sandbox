#pragma once

#include <SFML/Graphics.hpp>
#include <string>

void setupText(sf::Text& text, const sf::Font& font, const std::string& str, unsigned int size, sf::Vector2f pos, sf::Color color = sf::Color::White);
void renderUI(sf::RenderWindow& window, const sf::Font& font, float fps, bool overwriteMode);
void centerText(sf::Text& text, sf::Vector2f center, float y);