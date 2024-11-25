#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>
#include <string>

class HUD {
private:
    sf::Text topText;
    sf::RectangleShape expBarBackground;
    sf::RectangleShape expBar;
    sf::Text levelText;
    sf::Font font;

public:
    HUD(const std::string& fontPath);
    void update(int playerExp, int maxExp, int level, float elapsedTime);
    void draw(sf::RenderWindow& window, const sf::View& view);
    void loadFont(sf::Font& font, const std::string& filepath);
    void configureText(sf::Text& text, const sf::Font& font, const std::string& str, unsigned int charSize, sf::Color color, sf::Vector2f position);
};

#endif // HUD_HPP
