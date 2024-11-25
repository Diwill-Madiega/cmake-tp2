#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "Enemy.hpp"
#include "Fireball.hpp"
#include "Player.hpp"

class EntityManager {
private:
    sf::RenderWindow* window;
    std::vector<Enemy> enemies;
    std::vector<Fireball> fireballs;
    Player player;

public:
    EntityManager(sf::RenderWindow* window, const std::string& playerTexture);

    void handleInput(const sf::Event& event);
    void update(float deltaTime, int& playerExp, std::function<void()> onLevelUp);
    void render(sf::RenderWindow& window);

    void reset();
};

#endif // ENTITY_MANAGER_HPP