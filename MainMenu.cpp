#include "MainMenu.h"
#include "Constants.h"
#include "UI.h"

void renderMainMenu(sf::RenderWindow& window, const sf::Font& font)
{
    sf::Text title(font);
    sf::Text instructions(font);
	sf::Text exitInstructions(font);

    title.setString("Sand Sandbox");
    title.setCharacterSize(40);
    title.setFillColor(UIColors::Text);

    instructions.setString("Press Enter to Start");
    instructions.setCharacterSize(20);
    instructions.setFillColor(UIColors::Text);

    exitInstructions.setString("Press Esc to Exit");
    exitInstructions.setCharacterSize(20);
    exitInstructions.setFillColor(UIColors::Text);

    sf::Vector2f centerX = { WINDOW_WIDTH / 2.f, 0.f };

    centerText(title, centerX, 150.f);
    centerText(instructions, centerX, 300.f);
	centerText(exitInstructions, centerX, 375.f);

    window.clear(UIColors::Background);
    window.draw(title);
    window.draw(instructions);
    window.draw(exitInstructions);
    window.display();
}
