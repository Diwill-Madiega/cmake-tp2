#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <sstream>  // For std::ostringstream

#include "Player.hpp"
#include "Enemy.hpp"
#include "Fireball.hpp"

int main()
{
    sf::RenderWindow window({1920u, 1080u}, "Timeless Survivor");
    window.setFramerateLimit(144);

    sf::Font font;
    if (!font.loadFromFile("../../assets/dogicapixel.ttf")) {
        return -1;
    }

    sf::View view = window.getDefaultView();
    //view.zoom(0.5f);
    window.setView(view);

    Player player("../../assets/player.png");

    // Create a shared texture for all enemies
    auto enemyTexture = std::make_shared<sf::Texture>();
    if (!enemyTexture->loadFromFile("../../assets/enemy.png")) {
        std::cerr << "Error loading enemy texture\n";
        return -1;
    }

    auto fireballTexture = std::make_shared<sf::Texture>();
    if (!fireballTexture->loadFromFile("../../assets/fireball.png")) {
        std::cerr << "Error loading fireball texture\n";
        return -1;
    }

    std::vector<Enemy> enemies;
    std::vector<Fireball> fireballs;

    sf::Clock gameClock;  // Clock to keep track of the game's elapsed time
    sf::Clock deltaTimeClock;  // Used to calculate deltaTime
    sf::Clock enemySpawnClock; // Used for spawning enemies at regular intervals
    sf::Clock fireballCooldownClock; // Clock for fireball cooldown

    const float fireballCooldownTime = 0.6f;

    // Timer text setup
    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(50);
    timerText.setFillColor(sf::Color::White);
    timerText.setStyle(sf::Text::Bold);

    srand(static_cast<unsigned int>(time(nullptr)));  // Initialize random seed

    while (window.isOpen())
    {
        float deltaTime = deltaTimeClock.restart().asSeconds();

        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (fireballCooldownClock.getElapsedTime().asSeconds() >= fireballCooldownTime) {
                    fireballCooldownClock.restart();
                    Enemy* nearestEnemy = nullptr;
                    float nearestDistance = std::numeric_limits<float>::max();

                    for (auto& enemy : enemies) {
                        float dx = player.getPosition().x - enemy.getPosition().x;
                        float dy = player.getPosition().y - enemy.getPosition().y;
                        float distance = std::sqrt(dx * dx + dy * dy);

                        if (distance < nearestDistance) {
                            nearestDistance = distance;
                            nearestEnemy = &enemy;
                        }
                    }

                    if (nearestEnemy != nullptr) {
                        fireballs.emplace_back(fireballTexture, player.getPosition(), nearestEnemy->getPosition());
                        std::cout << "Fireball fired towards nearest enemy!\n";
                    }
                }
            }
        }

        // Regularly spawn enemies from the edges
        if (enemySpawnClock.getElapsedTime().asSeconds() > 1.5f) {
            enemySpawnClock.restart();

            int edge = rand() % 4;
            float spawnX = 0.0f, spawnY = 0.0f;

            switch (edge) {
                case 0: spawnX = static_cast<float>(rand() % window.getSize().x); spawnY = 0.0f; break;
                case 1: spawnX = static_cast<float>(window.getSize().x); spawnY = static_cast<float>(rand() % window.getSize().y); break;
                case 2: spawnX = static_cast<float>(rand() % window.getSize().x); spawnY = static_cast<float>(window.getSize().y); break;
                case 3: spawnX = 0.0f; spawnY = static_cast<float>(rand() % window.getSize().y); break;
                default: spawnX = static_cast<float>(rand() % window.getSize().x); spawnY = 0.0f; break;
            }

            enemies.emplace_back(enemyTexture, spawnX, spawnY);
        }

        // Update the elapsed time in the timer text
        float elapsedTime = gameClock.getElapsedTime().asSeconds();
        std::ostringstream ss;
        ss << "Time: " << static_cast<int>(elapsedTime) << "s";
        timerText.setString(ss.str());

        // Center the timer text at the top of the screen
        timerText.setPosition(window.getSize().x / 2.0f - timerText.getGlobalBounds().width / 2.0f, 20.0f);

        // Update player
        player.handleInput(deltaTime);

        // Update enemies to follow player
        for (auto& enemy : enemies) {
            enemy.follow(player.getPosition(), deltaTime);
        }

        // Check for collision between player and any enemy
        for (const auto& enemy : enemies) {
            if (player.getBounds().intersects(enemy.getBounds())) {
                std::cout << "Collision detected between player and enemy!\n";
                return 0;
            }
        }

        // Check for collision between fireballs and enemies
        for (auto& fireball : fireballs) {
            for (auto it = enemies.begin(); it != enemies.end();) {
                if (fireball.getBounds().intersects(it->getBounds())) {
                    std::cout << "Collision detected between fireball and enemy!\n";
                    it = enemies.erase(it);
                    fireball = fireballs.back();
                    fireballs.pop_back();
                    break;
                } else {
                    ++it;
                }
            }
        }

        // Update fireballs
        for (auto& fireball : fireballs) {
            fireball.update(deltaTime);
        }

        // Render
        window.clear(sf::Color::White);
        player.draw(window);

        for (const auto& enemy : enemies) {
            enemy.draw(window);
        }

        for (const auto& fireball : fireballs) {
            fireball.draw(window);
        }

        // Draw the timer text
        window.draw(timerText);

        window.display();
    }

    return 0;
}
