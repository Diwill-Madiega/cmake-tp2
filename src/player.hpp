#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    Player(const std::string& textureFile);
    void handleInput(float deltaTime);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    const sf::Vector2f& getPosition() const;
    const sf::FloatRect getBounds() const;
    void increaseSpeed(float amount);
    void reset();
    bool shield = false;
    bool invincible = false;

private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed = 150.f;

    sf::IntRect currentFrame;
    float animationTime = 0.f;
    float timePerFrame = 0.2f;
    int frameCount = 2;
    int frameWidth = 15;
    int frameHeight = 21;
};

#endif
