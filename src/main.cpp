#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <sstream>  // For std::ostringstream

#include "Player.hpp"
#include "Enemy.hpp"
#include "exp.hpp"
#include "Fireball.hpp"

int main()
{
    int level = 1;             // Player's current level
    const int maxExp = 10;     // EXP needed to level up
    int playerExp = 0;         // Current EXP
    bool gameOver = false;

    sf::RenderWindow window({1920u, 1080u}, "Timeless Survivor");
    window.setFramerateLimit(144);

    sf::Font font;
    if (!font.loadFromFile("../../assets/dogicapixel.ttf")) {
        return -1;
    }

    sf::View view = window.getDefaultView();
    view.zoom(0.5f);
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

    auto expTexture = std::make_shared<sf::Texture>();
    if (!expTexture->loadFromFile("../../assets/exp.png")) {
        std::cerr << "Error loading exp texture\n";
        return -1;
    }

    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("../../assets/ground.png")) {
        return -1;
    }
    tileTexture.setRepeated(true); // Allow the texture to repeat

    unsigned int scaleFactor = 2; // Example scale factor
    sf::Vector2u tileSize = tileTexture.getSize()*scaleFactor;

    // Create a vertex array for the tilemap
    sf::VertexArray tilemap(sf::Quads);

    // Calculate the number of tiles needed
    int tilesX = window.getSize().x / tileSize.x;
    int tilesY = window.getSize().y / tileSize.y;

    // Fill the vertex array
    for (int y = 0; y < tilesY; ++y) {
        for (int x = 0; x < tilesX; ++x) {
            sf::Vertex topLeft(sf::Vector2f(x * tileSize.x, y * tileSize.y), sf::Vector2f(0, 0));
            sf::Vertex topRight(sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y), sf::Vector2f(tileSize.x, 0));
            sf::Vertex bottomRight(sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y), sf::Vector2f(tileSize.x, tileSize.y));
            sf::Vertex bottomLeft(sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y), sf::Vector2f(0, tileSize.y));

            tilemap.append(topLeft);
            tilemap.append(topRight);
            tilemap.append(bottomRight);
            tilemap.append(bottomLeft);
        }
    }

    std::vector<Enemy> enemies;
    std::vector<Fireball> fireballs;
    std::vector<Exp> expPoints;

    sf::Clock gameClock;
    sf::Clock deltaTimeClock;
    sf::Clock enemySpawnClock;
    sf::Clock fireballCooldownClock;

    const float fireballCooldownTime = 0.6f;

    sf::Text topText;
    topText.setFont(font);
    topText.setCharacterSize(18);
    topText.setFillColor(sf::Color::White);
    topText.setPosition(view.getCenter().x - view.getSize().x / 2 + 10.f,
                        view.getCenter().y - view.getSize().y / 2 + 40.f);

    topText.setString("Timeless Survivor");

    srand(static_cast<unsigned int>(time(nullptr)));

    // EXP Bar background
    sf::RectangleShape expBarBackground;
    expBarBackground.setSize(sf::Vector2f(300.f, 20.f)); // Fixed background size
    expBarBackground.setFillColor(sf::Color(50, 50, 50)); // Dark gray color
    expBarBackground.setOutlineColor(sf::Color::White);
    expBarBackground.setOutlineThickness(2.f);
    expBarBackground.setPosition(view.getCenter().x - view.getSize().x / 2 + 10.f,
                        view.getCenter().y - view.getSize().y / 2 + 10.f); // Position below the top text

    // EXP Bar setup
    sf::RectangleShape expBar;
    expBar.setSize(sf::Vector2f(200.f, 20.f)); // Initial size of the bar
    expBar.setFillColor(sf::Color(255,190,0));
    expBar.setPosition(view.getCenter().x - view.getSize().x / 2 + 10.f,
                        view.getCenter().y - view.getSize().y / 2 + 10.f); // Position below the top text

    sf::Text levelText;
    levelText.setFont(font);
    levelText.setCharacterSize(12);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(view.getCenter().x - view.getSize().x / 2 + 108.f,
                        view.getCenter().y - view.getSize().y / 2 + 13.f); // Position below the top text
    levelText.setString("Level 1");   // Initial text

    // game over screen
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(view.getCenter().x - view.getSize().x / 2,
    view.getCenter().y - view.getSize().y / 2); // Position below the top text


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

        player.handleInput(deltaTime);

        for (auto& enemy : enemies) {
            enemy.follow(player.getPosition(), deltaTime);
        }

        for (const auto& exp : expPoints) {
            for (auto it = expPoints.begin(); it != expPoints.end();) {
                if (player.getBounds().intersects(it->getBounds())) {
                    it = expPoints.erase(it);
                    playerExp++;
                    break;
                } else {
                    ++it;
                }
            }
        }

        for (auto& fireball : fireballs) {
            for (auto it = enemies.begin(); it != enemies.end();) {
                if (fireball.getBounds().intersects(it->getBounds())) {
                    expPoints.emplace_back(expTexture, it->getPosition().x, it->getPosition().y);
                    it = enemies.erase(it);
                    fireball = fireballs.back();
                    fireballs.pop_back();
                    break;
                } else {
                    ++it;
                }
            }
        }

        for (auto& fireball : fireballs) {
            fireball.update(deltaTime);
        }

        for (auto& enemy : enemies) {
            enemy.update(deltaTime);
        }

        std::ostringstream oss;
        int elapsedSeconds = static_cast<int>(gameClock.getElapsedTime().asSeconds());
        int minutes = elapsedSeconds / 60;
        int seconds = elapsedSeconds % 60;

        oss << (minutes < 10 ? "0" : "") << minutes << ":"
            << (seconds < 10 ? "0" : "") << seconds;

        topText.setString(oss.str());

        // Update EXP bar size
        float maxExpBarWidth = expBarBackground.getSize().x; // Use background width as max
        expBar.setSize(sf::Vector2f((playerExp / static_cast<float>(maxExp)) * maxExpBarWidth, 20.f));

        // Check for level-up
        if (playerExp >= maxExp) {
            ++level;               // Increase level
            playerExp = 0;         // Reset EXP

            // Update level text
            levelText.setString("Level " + std::to_string(level));
        }

        // Check for collision between player and any enemy
        for (const auto& enemy : enemies) {
            for (auto it = enemies.begin(); it != enemies.end();) {
                if (player.getBounds().intersects(it->getBounds())) {
                    std::cout << "Collision detected between player and enemy!\n";
                    it = enemies.erase(it);
                    gameOverText.setString("Game Over");   // Initial text
                    window.clear();
                    window.draw(gameOverText);
                    window.display();
                    gameOver = true;
                    std::cin.ignore();
                    return 0;
                } else {
                    ++it;
                }
            }
        }

        if (!gameOver)
        {
            window.clear();

            sf::RenderStates states;
            states.texture = &tileTexture;
            window.draw(tilemap, states);

            for (const auto& exp : expPoints) {
                exp.draw(window);
            }

            for (const auto& enemy : enemies) {
                enemy.draw(window);
            }

            for (const auto& fireball : fireballs) {
                fireball.draw(window);
            }

            player.draw(window);

            window.draw(topText);
            window.draw(expBarBackground);
            window.draw(expBar); // Draw the EXP bar
            window.draw(levelText);        // Draw the level text

            window.display();
        }
    }
    return 0;
}
