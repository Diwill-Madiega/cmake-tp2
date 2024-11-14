#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    Player(const std::string& textureFile);
    void handleInput(float deltaTime);
    void draw(sf::RenderWindow& window);
    const sf::Vector2f& getPosition() const;
    const sf::FloatRect getBounds() const;

private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed = 300.f;
};

#endif // PLAYER_HPP