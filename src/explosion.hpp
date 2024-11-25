#include <SFML/Graphics.hpp>

class Explosion {
public:
    Explosion(std::shared_ptr<sf::Texture> texture, const sf::Vector2f& position, float duration)
        : sprite(*texture), lifetime(duration) {
        sprite.setPosition(position);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
        sprite.setScale(2.f, 2.f); // Scale to make it more visible
    }

    void update(float deltaTime) {
        lifetime -= deltaTime;
    }

    bool isExpired() const {
        return lifetime <= 0.f;
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(sprite);
    }

private:
    sf::Sprite sprite;
    float lifetime; // Remaining time before the explosion disappears
};
