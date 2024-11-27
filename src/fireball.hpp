#ifndef FIREBALL_HPP
#define FIREBALL_HPP

#include <SFML/Graphics.hpp>

class Fireball {
public:
    Fireball(std::shared_ptr<sf::Texture> texture, const sf::Vector2f& startPos, const sf::Vector2f& targetPos);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    const sf::FloatRect getBounds() const;
    bool isOffScreen(const sf::RenderWindow& window) const;

private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
    sf::Vector2f direction;
    float speed = 500.0f;
};

#endif