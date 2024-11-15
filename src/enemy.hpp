#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Enemy {
public:
    Enemy(std::shared_ptr<sf::Texture> texture, float spawnX, float spawnY);

    void update(float deltaTime);
    void follow(const sf::Vector2f& targetPosition, float deltaTime);
    void draw(sf::RenderWindow& window) const;
    const sf::FloatRect getBounds() const;
    const sf::Vector2f& getPosition() const;

private:
    std::shared_ptr<sf::Texture> texture;  // Shared pointer for texture
    sf::Sprite sprite;
    float speed = 100.0f;  // Movement speed

    // Animation-related variables
    sf::IntRect currentFrame;
    float animationTime = 0.f;
    float timePerFrame = 0.3f; // Adjust as needed
    int frameCount = 2;       // Number of frames in your sprite sheet (horizontal)
    int frameWidth = 12;      // Width of a single frame
    int frameHeight = 10;     // Height of a single frame
};

#endif
