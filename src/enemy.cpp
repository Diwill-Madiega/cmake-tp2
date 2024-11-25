// Enemy.cpp
#include "Enemy.hpp"
#include <cmath>
#include <iostream>

Enemy::Enemy(std::shared_ptr<sf::Texture> sharedTexture, float spawnX, float spawnY)
    : texture(std::move(sharedTexture))  // Initialize the shared texture
{
    sprite.setTexture(*texture);
    sprite.setScale(2.f, 2.f);
    sprite.setPosition(spawnX, spawnY);  // Set initial position based on parameters
}

void Enemy::follow(const sf::Vector2f& targetPosition, float deltaTime)
{
    sf::Vector2f direction = targetPosition - sprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 1.f)
    {
        direction /= distance;  // Normalize
        sprite.move(direction * speed * deltaTime);
    }
}

void Enemy::update(float deltaTime)
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

        currentFrame = sf::IntRect(0, 0, frameWidth, frameHeight);
        sprite.setTextureRect(currentFrame);
    }
}

void Enemy::draw(sf::RenderWindow& window) const
{
    window.draw(sprite);
}

const sf::FloatRect Enemy::getBounds() const
{
    return sprite.getGlobalBounds();
}

const sf::Vector2f& Enemy::getPosition() const
{
    return sprite.getPosition();
}
