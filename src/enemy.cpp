// Enemy.cpp
#include "Enemy.hpp"
#include <cmath>
#include <iostream>

Enemy::Enemy(std::shared_ptr<sf::Texture> sharedTexture, float spawnX, float spawnY)
    : texture(std::move(sharedTexture))  // Initialize the shared texture
{
    if (!texture || !texture->loadFromFile("../../assets/enemy.png")) {
        std::cerr << "Error loading enemy texture\n";
    }
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
