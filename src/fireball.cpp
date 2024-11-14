// Fireball.cpp
#include "Fireball.hpp"
#include <cmath>
#include <iostream>

Fireball::Fireball(std::shared_ptr<sf::Texture> sharedTexture, const sf::Vector2f& startPos, const sf::Vector2f& targetPos)
    : texture(std::move(sharedTexture))  // Initialize the shared texture
{
    sprite.setTexture(*texture);
    sprite.setPosition(startPos);
    sprite.setScale(2.f, 2.f);

    // Calculate the direction vector and normalize it
    direction = targetPos - startPos;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) direction /= length;
}

void Fireball::update(float deltaTime) {
    // Move the fireball in the direction of the target
    sprite.move(direction * speed * deltaTime);
}

void Fireball::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

const sf::FloatRect Fireball::getBounds() const {
    return sprite.getGlobalBounds();
}

bool Fireball::isOffScreen(const sf::RenderWindow& window) const {
    return !window.getView().getViewport().contains(sprite.getPosition());
}

