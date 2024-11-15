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

private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed = 200.f;

    // Animation-related variables
    sf::IntRect currentFrame;
    float animationTime = 0.f;
    float timePerFrame = 0.2f; // Adjust as needed
    int frameCount = 2;       // Number of frames in your sprite sheet (horizontal)
    int frameWidth = 15;      // Width of a single frame
    int frameHeight = 21;     // Height of a single frame
};

#endif // PLAYER_HPP
