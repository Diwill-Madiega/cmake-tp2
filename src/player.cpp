#include "Player.hpp"
#include <iostream>

Player::Player(const std::string& textureFile)
{
    if (!texture.loadFromFile(textureFile))
    {
        std::cerr << "Error loading player texture\n";
    }
    sprite.setTexture(texture);
    sprite.setScale(2.f, 2.f);
    sprite.setPosition(960.f, 540.f); // Initial position (center screen)
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

const sf::FloatRect Player::getBounds() const
{
    return sprite.getGlobalBounds();
}
