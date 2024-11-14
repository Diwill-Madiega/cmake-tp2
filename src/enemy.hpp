#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Enemy {
public:
    Enemy(std::shared_ptr<sf::Texture> texture, float spawnX, float spawnY);

    void follow(const sf::Vector2f& targetPosition, float deltaTime);
    void draw(sf::RenderWindow& window) const;
    const sf::FloatRect getBounds() const;
    const sf::Vector2f& getPosition() const;

private:
    std::shared_ptr<sf::Texture> texture;  // Shared pointer for texture
    sf::Sprite sprite;
    float speed = 100.0f;  // Movement speed
};

#endif
