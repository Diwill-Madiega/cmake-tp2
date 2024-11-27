#include "Player.hpp"

Player::Player(const std::string& textureFile)
{
    texture.loadFromFile(textureFile);
    sprite.setTexture(texture);

    frameWidth = texture.getSize().x / frameCount;
    frameHeight = texture.getSize().y;
    currentFrame = sf::IntRect(0, 0, frameWidth, frameHeight);
    sprite.setTextureRect(currentFrame);

    sprite.setScale(2.f, 2.f);
    sprite.setPosition(960.f, 540.f);
}

void Player::update(float deltaTime)
{
    animationTime += deltaTime;
    if (animationTime >= timePerFrame)
    {
        animationTime -= timePerFrame;

        currentFrame.left += frameWidth;
        if (currentFrame.left >= frameCount * frameWidth)
            currentFrame.left = 0;

        sprite.setTextureRect(currentFrame);
    }
}

void Player::handleInput(float deltaTime)
{
    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        movement.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movement.y += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        movement.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        movement.x += speed * deltaTime;

    if (movement != sf::Vector2f(0.f, 0.f))
        update(deltaTime);

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
    float shrinkFactor = 0.7f;
    float reducedWidth = bounds.width * (1.0f - shrinkFactor);
    float reducedHeight = bounds.height * (1.0f - shrinkFactor);
    return sf::FloatRect(
        bounds.left + (bounds.width - reducedWidth) / 2.0f,
        bounds.top + (bounds.height - reducedHeight) / 2.0f,
        reducedWidth,
        reducedHeight
    );
}

void Player::increaseSpeed(float amount) {
    speed += amount;
}

void Player::reset()
{
    speed = 150.f;
    sprite.setPosition(960.f, 540.f);
}