// Exp.hpp
#pragma once
#include <SFML/Graphics.hpp>

class Exp {
public:
    Exp(std::shared_ptr<sf::Texture> texture, float x, float y)
        : sprite(*texture) {
        sprite.setPosition(x, y);
        sprite.setScale(2.f, 2.f);
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(sprite);
    }

    const sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }

private:
    sf::Sprite sprite;
};

