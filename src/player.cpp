#include "Player.hpp"
#include <iostream>

Player::Player(const std::string& textureFile)
{
    if (!texture.loadFromFile(textureFile))
    {
        std::cerr << "Error loading player texture\n";
    }
    sprite.setTexture(texture);

    // Initialize animation frame
    frameWidth = texture.getSize().x / frameCount; // Calculate frame width dynamically
    frameHeight = texture.getSize().y;            // Full height of the sprite
    currentFrame = sf::IntRect(0, 0, frameWidth, frameHeight);
    sprite.setTextureRect(currentFrame);

    sprite.setScale(2.f, 2.f);
    sprite.setPosition(960.f, 540.f); // Initial position (center screen)
}

void Player::update(float deltaTime)
{
    animationTime += deltaTime;
    if (animationTime >= timePerFrame)
    {
        animationTime -= timePerFrame;

        // Move to the next frame
        currentFrame.left += frameWidth;
        if (currentFrame.left >= frameCount * frameWidth)
        {
            currentFrame.left = 0; // Loop back to the first frame
        }

        sprite.setTextureRect(currentFrame);
    }
}


void Player::handleInput(float deltaTime)
{
    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        movement.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        movement.y += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        movement.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        movement.x += speed * deltaTime;

    if (movement != sf::Vector2f(0.f, 0.f))
    {
        update(deltaTime);
    }

    sprite.move(movement);
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

const sf::Vector2f& Player::getPosition() const
{
    return sprite.getPosition();
}

const sf::FloatRect Player::getBounds() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    float shrinkFactor = 0.7f; // Adjust to your desired size
    float reducedWidth = bounds.width * (1.0f - shrinkFactor);
    float reducedHeight = bounds.height * (1.0f - shrinkFactor);
    return sf::FloatRect(
        bounds.left + (bounds.width - reducedWidth) / 2.0f,
        bounds.top + (bounds.height - reducedHeight) / 2.0f,
        reducedWidth,
        reducedHeight
    );
}
