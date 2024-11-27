#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(std::shared_ptr<sf::Texture> texture, float spawnX, float spawnY);

    void update(float deltaTime);
    void follow(const sf::Vector2f& targetPosition, float deltaTime);
    void draw(sf::RenderWindow& window) const;
    const sf::FloatRect getBounds() const;
    const sf::Vector2f& getPosition() const;

private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
    float speed = 100.0f;

    sf::IntRect currentFrame;
    float animationTime = 0.f;
    float timePerFrame = 0.3f;
    int frameCount = 2;
    int frameWidth = 12;
    int frameHeight = 10;
};

#endif
