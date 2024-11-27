#include "HUD.hpp"
#include <sstream>

HUD::HUD(const std::string& fontPath) {
    font.loadFromFile(fontPath);

    topText.setFont(font);
    topText.setCharacterSize(18);
    topText.setFillColor(sf::Color::White);

    expBarBackground.setSize(sf::Vector2f(300.f, 20.f));
    expBarBackground.setFillColor(sf::Color(50, 50, 50));
    expBarBackground.setOutlineColor(sf::Color::White);
    expBarBackground.setOutlineThickness(2.f);

    expBar.setFillColor(sf::Color(255, 190, 0));

    levelText.setFont(font);
    levelText.setCharacterSize(12);
    levelText.setFillColor(sf::Color::White);
}

void HUD::update(int playerExp, int maxExp, int level, float elapsedTime) {

    std::ostringstream oss;
    int elapsedSeconds = static_cast<int>(elapsedTime);
    int minutes = elapsedSeconds / 60;
    int seconds = elapsedSeconds % 60;

    oss << (minutes < 10 ? "0" : "") << minutes << ":"
        << (seconds < 10 ? "0" : "") << seconds;
    topText.setString(oss.str());

    float maxExpBarWidth = expBarBackground.getSize().x;
    expBar.setSize(sf::Vector2f((playerExp / static_cast<float>(maxExp)) * maxExpBarWidth, 20.f));
    levelText.setString("Level " + std::to_string(level));
}

void HUD::draw(sf::RenderWindow& window, const sf::View& view) {
    sf::Vector2f viewTopLeft(view.getCenter().x - view.getSize().x / 2,view.getCenter().y - view.getSize().y / 2);

    topText.setPosition(viewTopLeft.x + 10.f, viewTopLeft.y + 40.f);
    expBarBackground.setPosition(viewTopLeft.x + 10.f, viewTopLeft.y + 10.f);
    expBar.setPosition(expBarBackground.getPosition());
    levelText.setPosition(viewTopLeft.x + 108.f, viewTopLeft.y + 13.f);

    window.draw(topText);
    window.draw(expBarBackground);
    window.draw(expBar);
    window.draw(levelText);
}

void HUD::loadFont(sf::Font& font, const std::string& filepath) {
    font.loadFromFile(filepath);
}

void HUD::configureText(sf::Text& text, const sf::Font& font, const std::string& str, unsigned int charSize, sf::Color color, sf::Vector2f position) {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(charSize);
    text.setFillColor(color);
    text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
    text.setPosition(position);
}