#include "Game.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <SFML/Audio.hpp>
#include <thread>
#include <SFML/Graphics.hpp>
#include <random>

Game::Game()
    : window({1920u, 1080u}, "Timeless Survivor", sf::Style::Fullscreen),
      view(window.getDefaultView()),
      player("../../assets/player.png"),
      hud("../../assets/dogicapixel.ttf"),
      tilemap("../../assets/ground.png", window.getSize(), 2),
      level(1), playerExp(0), maxExp(5), gameOver(false),
      fireballCooldownTime(0.6f),
      enemySpawnInterval(1.5f) {
    window.setFramerateLimit(144);
    view.zoom(0.5f);
    window.setView(view);



    hud.loadFont(hudFont, "../../assets/dogicapixel.ttf");
    hud.configureText(levelUpText, hudFont, "LEVEL UP!", 48, sf::Color::White, {view.getCenter().x, view.getCenter().y - view.getSize().y / 2.f + 50.f});

    // Load the icon
    sf::Image icon;
    if (!icon.loadFromFile("../../assets/icon.png")) {
        // Handle error (file not found, etc.)
    }

    // Set the icon of the window
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load the main theme music
    if (!mainTheme.openFromFile("../../assets/main_theme.mp3")) {
        throw std::runtime_error("Error loading main theme music.");
    }
    mainTheme.setLoop(true);  // Enable looping
    mainTheme.setVolume(50.f); // Set volume (0 to 100)
    mainTheme.play(); // Start playing the music

    // Pause menu background
    pauseBackground.setSize(sf::Vector2f(window.getSize()));
    pauseBackground.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black

    // Resume button
    resumeButton.setSize({200.f, 50.f});
    resumeButton.setFillColor(sf::Color::Green);
    resumeButton.setOrigin(resumeButton.getSize().x / 2.f, resumeButton.getSize().y / 2.f);
    resumeButton.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f - 50.f);

    hud.configureText(resumeText, hudFont, "Resume", 24, sf::Color::White, resumeButton.getPosition());

    //Pause Quit button
    pauseQuitButton.setSize({200.f, 50.f});
    pauseQuitButton.setFillColor(sf::Color::Red);
    pauseQuitButton.setOrigin(pauseQuitButton.getSize().x / 2.f, pauseQuitButton.getSize().y / 2.f);
    pauseQuitButton.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f + 50.f);

    hud.configureText(pauseQuitText, hudFont, "Quit", 24, sf::Color::White, pauseQuitButton.getPosition());

    shieldTexture = loadTexture("../../assets/shield.png"); // Replace with your shield texture path
    shieldSprite.setTexture(*shieldTexture);
    shieldSprite.setOrigin(
        shieldSprite.getLocalBounds().width / 2.f,
        shieldSprite.getLocalBounds().height / 2.f
    );
    shieldSprite.setScale(2.f, 2.f); // Scale the shield sprite

    initializeLevelUpButtons();
    enemyTexture = loadTexture("../../assets/enemy.png");
    fireballTexture = loadTexture("../../assets/fireball.png");
    expTexture = loadTexture("../../assets/exp.png");
    explosionTexture = loadTexture("../../assets/explosion.png");

    if (!fireSoundBuffer.loadFromFile("../../assets/fire.wav")) {
        throw std::runtime_error("Failed to load fire.wav");
    }
    fireSound.setBuffer(fireSoundBuffer);

    if (!hitSoundBuffer.loadFromFile("../../assets/hit.wav")) {
        throw std::runtime_error("Failed to load hit.wav");
    }
    hitSound.setBuffer(hitSoundBuffer);

    if (!deathSoundBuffer.loadFromFile("../../assets/death.wav")) {
        throw std::runtime_error("Failed to load death.wav");
    }
    deathSound.setBuffer(deathSoundBuffer);

    if (!expSoundBuffer.loadFromFile("../../assets/pickup.wav")) {
        throw std::runtime_error("Failed to load pickup.wav");
    }
    expSound.setBuffer(expSoundBuffer);

    if (!levelUpSoundBuffer.loadFromFile("../../assets/level_up.wav")) {
        throw std::runtime_error("Failed to load level_up.wav");
    }
    levelUpSound.setBuffer(levelUpSoundBuffer);


    const std::string buttonTextureFiles[3] = {
        "../../assets/speed_button.png",
        "../../assets/firerate_button.png",
        "../../assets/defense_button.png"
    };

    // Load textures and configure buttons
    for (int i = 0; i < 3; ++i) {
        // Load the texture
        buttonTextures[i] = loadTexture(buttonTextureFiles[i]);

        // Configure the sprite
        levelUpButtonSprites[i].setTexture(*buttonTextures[i]);
        levelUpButtonSprites[i].setScale(5.f, 5.f); // Adjust scale as needed
        levelUpButtonSprites[i].setOrigin(
            levelUpButtonSprites[i].getLocalBounds().width / 2.f,
            levelUpButtonSprites[i].getLocalBounds().height
        );

        // Position the sprite (spread out horizontally)
        levelUpButtonSprites[i].setPosition(
            (window.getSize().x / 2.f) + (i - 1) * 200.f, // Horizontal spacing
            window.getSize().y / 2.f
        );

        // Configure button text
        buttonTexts[i].setFont(hudFont);
        buttonTexts[i].setCharacterSize(16);
        buttonTexts[i].setFillColor(sf::Color::White);
        buttonTexts[i].setOrigin(
            buttonTexts[i].getLocalBounds().width / 2.f,
            buttonTexts[i].getLocalBounds().height / 2.f
        );
        buttonTexts[i].setPosition(
            levelUpButtonSprites[i].getPosition().x,
            levelUpButtonSprites[i].getPosition().y + 10.f // Offset for alignment
        );
    }
    configureGameOverUI();
    srand(static_cast<unsigned int>(time(nullptr)));
}

// Initialize level-up buttons
void Game::initializeLevelUpButtons() {
    const std::string buttonLabels[3] = {"Speed", "Firerate", "Shield"};
    const std::string buttonTextureFiles[3] = {
        "../../assets/speed_button.png",
        "../../assets/firerate_button.png",
        "../../assets/defense_button.png"
    };

    const float buttonSpacing = 200.f;
    const sf::Vector2f center = {window.getSize().x / 2.f, window.getSize().y / 2.f};

    for (int i = 0; i < 3; ++i) {
        // Load texture
        buttonTextures[i] = loadTexture(buttonTextureFiles[i]);

        // Configure sprite
        levelUpButtonSprites[i].setTexture(*buttonTextures[i]);
        levelUpButtonSprites[i].setScale(5.f, 5.f);
        levelUpButtonSprites[i].setOrigin(
            levelUpButtonSprites[i].getLocalBounds().width / 2.f,
            levelUpButtonSprites[i].getLocalBounds().height
        );
        levelUpButtonSprites[i].setPosition(center.x + (i - 1) * buttonSpacing, center.y);

        // Configure text
        buttonTexts[i].setFont(hudFont);
        buttonTexts[i].setString(buttonLabels[i]);
        buttonTexts[i].setCharacterSize(16);
        buttonTexts[i].setFillColor(sf::Color::White);
        buttonTexts[i].setOrigin(buttonTexts[i].getLocalBounds().width / 2.f, buttonTexts[i].getLocalBounds().height / 2.f);
        buttonTexts[i].setPosition(levelUpButtonSprites[i].getPosition().x, levelUpButtonSprites[i].getPosition().y + 10.f);
    }
}

std::shared_ptr<sf::Texture> Game::loadTexture(const std::string& filepath) {
    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(filepath)) {
        throw std::runtime_error("Error loading texture: " + filepath);
    }
    return texture;
}

// Configure Game Over UI
void Game::configureGameOverUI() {
    hud.configureText(gameOverText, hudFont, "GAME OVER", 48, sf::Color::Red, {window.getSize().x / 2.f, window.getSize().y / 3.f});
    hud.configureText(finalTimeText, hudFont, "Final Time : 1s", 26, sf::Color::White, {window.getSize().x / 2.f, window.getSize().y / 2.5f});

    restartButton.setSize({200.f, 50.f});
    restartButton.setFillColor(sf::Color::Green);
    restartButton.setOrigin(restartButton.getSize().x / 2.f, restartButton.getSize().y / 2.f);
    restartButton.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f + 75.f);

    quitButton.setSize({200.f, 50.f});
    quitButton.setFillColor(sf::Color::Red);
    quitButton.setOrigin(quitButton.getSize().x / 2.f, quitButton.getSize().y / 2.f);
    quitButton.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f + 150.f);

    hud.configureText(restartText, hudFont, "Restart", 24, sf::Color::White, restartButton.getPosition());
    hud.configureText(quitText, hudFont, "Quit", 24, sf::Color::White, quitButton.getPosition());
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = deltaTimeClock.restart().asSeconds();
        handleEvents();
        if (!gameOver) update(deltaTime);
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Toggle pause
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            paused = !paused;
        }

        if (paused) {
            mainTheme.setVolume(15.f); // Set volume (0 to 100)
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

                if (resumeButton.getGlobalBounds().contains(mousePos)) {
                    paused = false; // Resume game
                } else if (pauseQuitButton.getGlobalBounds().contains(mousePos)) {
                    window.close(); // Quit game
                }
            }
            return; // Skip other event handling while paused
        }
        else
            mainTheme.setVolume(50.f); // Set volume (0 to 100)

        if (gameOver) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

                if (restartButton.getGlobalBounds().contains(mousePos)) {
                    restartGame();
                } else if (quitButton.getGlobalBounds().contains(mousePos)) {
                    window.close();
                }
            }
            return;
        }

        if (isLevelUpPaused && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
            for (int i = 0; i < 3; ++i) {
                if (levelUpButtonSprites[i].getGlobalBounds().contains(mousePos)) {
                    switch (i) {
                    case 0:
                        player.increaseSpeed(50.f);
                        break;
                    case 1:
                            fireballCooldownTime -= 0.1f; // Reduce by 0.1 seconds
                        if (fireballCooldownTime < 0.2f)
                            fireballCooldownTime = 0.2f; // Ensure it doesn't go below a safe limit
                        break;
                    case 2:
                        std::cout<<"you got a shield!!";
                        player.shield = true;
                        break;
                    }
                    isLevelUpPaused = false; // Resume the game
                    break;
                }
            }
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            isLeftMouseButtonHeld = true;
        }
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            isLeftMouseButtonHeld = false;
        }
    }

    // Check if the left mouse button is held and the cooldown has elapsed
    if (isLeftMouseButtonHeld && !isLevelUpPaused && !gameOver) {
        spawnFireball();
    }
}


void Game::update(float deltaTime) {

    if (paused || gameOver || isLevelUpPaused) return;

    for (auto it = explosions.begin(); it != explosions.end();) {
        it->lifetime -= deltaTime;
        if (it->lifetime <= 0.f) {
            it = explosions.erase(it);
        } else {
            ++it;
        }
    }

    if (player.invincible && iframeClock.getElapsedTime().asSeconds() >= invincibilityDuration) {
        player.invincible = false;
    }

    if (player.shield) {
        shieldSprite.setPosition(
            player.getPosition().x + player.getBounds().width * 1.65f,
            player.getPosition().y + player.getBounds().height * 1.65f
        );
    }

    player.handleInput(deltaTime);

    for (auto& enemy : enemies) {
        enemy.follow(player.getPosition(), deltaTime);
        enemy.update(deltaTime);
    }

    for (auto& fireball : fireballs) {
        fireball.update(deltaTime);
    }

    for (auto it = expPoints.begin(); it != expPoints.end();) {
        if (player.getBounds().intersects(it->getBounds())) {
            it = expPoints.erase(it);
            playerExp++;
            expSound.play();
        } else {
            ++it;
        }
    }

    // Dynamically adjust enemy spawn interval
    const float maxSpawnInterval = 1.5f; // Starting interval
    const float minSpawnInterval = 0.2f; // Minimum allowed interval
    const float spawnDecrementRate = 0.01f; // Decrease rate (seconds per second)

    enemySpawnInterval = std::max(minSpawnInterval, maxSpawnInterval - gameClock.getElapsedTime().asSeconds() * spawnDecrementRate);

    if (enemySpawnClock.getElapsedTime().asSeconds() > enemySpawnInterval) {
        spawnEnemy();
        enemySpawnClock.restart();
    }
    handleCollisions();
    checkLevelUp();
}

void Game::render() {
    window.clear();
    tilemap.draw(window);

    if (!gameOver) {
        for (const auto& exp : expPoints) exp.draw(window);
        for (const auto& enemy : enemies) enemy.draw(window);
        player.draw(window);
        if (player.shield) window.draw(shieldSprite);
        for (const auto& fireball : fireballs) fireball.draw(window);

        for (const auto& explosion : explosions) {
            window.draw(explosion.sprite);
        }

        hud.draw(window, view);

        if (isLevelUpPaused) {
            window.draw(levelUpText);
            for (int i = 0; i < 3; ++i) {
                window.draw(levelUpButtonSprites[i]);
                window.draw(buttonTexts[i]);
            }
        }

        if (paused) {
            window.draw(pauseBackground);
            window.draw(resumeButton);
            window.draw(pauseQuitButton);
            window.draw(resumeText);
            window.draw(pauseQuitText);
        }
    } else {
        window.draw(gameOverText);
        mainTheme.stop();
        std::ostringstream timeStream;

        if (finalTime >= 0) {  // Ensure it's a valid time
            timeStream << "Final Time: " << static_cast<int>(finalTime) << "s";
        }
        finalTimeText.setString(timeStream.str());
        finalTimeText.setPosition(window.getSize().x / 2.f, window.getSize().y / 3.f + 100.f);
        window.draw(finalTimeText);

        window.draw(restartButton);
        window.draw(quitButton);
        window.draw(restartText);
        window.draw(quitText);
    }
    window.display();
}

void Game::spawnEnemy() {
    int edge = rand() % 4;
    float spawnX = 0.0f, spawnY = 0.0f;

    switch (edge) {
        case 0: spawnX = static_cast<float>(rand() % window.getSize().x); spawnY = 0.0f; break;
        case 1: spawnX = static_cast<float>(window.getSize().x); spawnY = static_cast<float>(rand() % window.getSize().y); break;
        case 2: spawnX = static_cast<float>(rand() % window.getSize().x); spawnY = static_cast<float>(window.getSize().y); break;
        case 3: spawnX = 0.0f; spawnY = static_cast<float>(rand() % window.getSize().y); break;
    }
    enemies.emplace_back(enemyTexture, spawnX, spawnY);
}

void Game::spawnFireball() {
    if (fireballCooldownClock.getElapsedTime().asSeconds() >= fireballCooldownTime) {
        fireballCooldownClock.restart();

     //   sound.play();
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

        if (nearestEnemy) {
            sf::Vector2f fireballSpawnPos = player.getPosition();
            fireballSpawnPos.x += player.getBounds().width / 2.f;
            fireballSpawnPos.y += player.getBounds().height / 2.f;

            fireballs.emplace_back(fireballTexture, fireballSpawnPos, nearestEnemy->getPosition());
            fireSound.play();
        }
    }
}

void Game::handleCollisions() {
    // Fireball-enemy collisions
    for (auto fireballIt = fireballs.begin(); fireballIt != fireballs.end();) {
        bool fireballDestroyed = false;

        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
            if (fireballIt->getBounds().intersects(enemyIt->getBounds())) {
                // Calculate centered position for experience spawn
                sf::Vector2f expSpawnPos = enemyIt->getPosition();
                expSpawnPos.x += enemyIt->getBounds().width / 4.f;
                expSpawnPos.y += enemyIt->getBounds().height / 4.f;

                expPoints.emplace_back(expTexture, expSpawnPos.x, expSpawnPos.y);

                // Spawn explosion at the enemy's position
                sf::Vector2f explosionPos = enemyIt->getPosition();
                explosionPos.x += enemyIt->getBounds().width / 2.f;
                explosionPos.y += enemyIt->getBounds().height / 2.f;

                Explosion explosion;
                explosion.sprite.setTexture(*explosionTexture);
                explosion.sprite.setPosition(explosionPos);
                explosion.sprite.setOrigin(
                    explosion.sprite.getLocalBounds().width / 2.f,
                    explosion.sprite.getLocalBounds().height / 2.f
                );
                explosion.lifetime = explosionDuration;
                explosion.sprite.setScale(2.f, 2.f); // Scale the explosion
                explosions.push_back(explosion);
                hitSound.play();
                enemyIt = enemies.erase(enemyIt);
                fireballIt = fireballs.erase(fireballIt);
                fireballDestroyed = true;
                break;
            } else {
                ++enemyIt;
            }
        }
        if (!fireballDestroyed) ++fireballIt;
    }

    // Player-enemy collisions
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (player.getBounds().intersects(it->getBounds()) && !player.invincible) {
            if (!player.shield && !player.invincible) {
                if (finalTime == -1) {  // Check if finalTime is not set yet
                    finalTime = gameClock.getElapsedTime().asSeconds();  // Save the final time
                }
                deathSound.play();
                gameOver = true;
                return;
            }
            if (player.shield) {
                IFrames();
                player.shield = false;
                std::cout << "You lost shield...\n";
                it = enemies.erase(it);
            }

            // Remove the enemy

        } else {
            ++it;  // Only increment the iterator if no enemy was removed
        }
    }
}

void Game::checkLevelUp() {
    if (playerExp >= maxExp) {
        level++;
        playerExp = 0;
        maxExp = 5 + level * 3; // Adjust 5 and 3 as per your desired growth rate
        levelUpSound.play();
        isLevelUpPaused = true; // Pause the game
    }
    hud.update(playerExp, maxExp, level, gameClock.getElapsedTime().asSeconds());
}

void Game::restartGame() {
    enemies.clear();
    fireballs.clear();
    expPoints.clear();
    player.reset();

    playerExp = 0;
    level = 1;
    maxExp = 5;
    finalTime = -1;
    gameClock.restart();
    enemySpawnClock.restart();
    gameOver = false;

    mainTheme.play();
}

void Game::IFrames() {
    player.invincible = true;
    iframeClock.restart(); // Start the clock to track invincibility duration
}
