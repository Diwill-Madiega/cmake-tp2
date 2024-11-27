#include "Game.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <random>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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

    sf::Image icon;
    icon.loadFromFile("../../assets/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    loadTextures();
    initializeLevelUpButtons();
    loadSounds();
    configureUI();
    srand(static_cast<unsigned int>(time(nullptr)));
}

void Game::loadTextures() {

    shieldTexture = loadTexture("../../assets/shield.png");
    shieldSprite.setTexture(*shieldTexture);
    shieldSprite.setOrigin(shieldSprite.getLocalBounds().width / 2.f,shieldSprite.getLocalBounds().height / 2.f);
    shieldSprite.setScale(2.f, 2.f);

    enemyTexture = loadTexture("../../assets/enemy.png");
    fireballTexture = loadTexture("../../assets/fireball.png");
    expTexture = loadTexture("../../assets/exp.png");
    explosionTexture = loadTexture("../../assets/explosion.png");

    buttonTextures[0] = loadTexture("../../assets/speed_button.png");
    buttonTextures[1] = loadTexture("../../assets/firerate_button.png");
    buttonTextures[2] = loadTexture("../../assets/defense_button.png");
}

void Game::loadSounds()
{
    if (!mainTheme.openFromFile("../../assets/main_theme.mp3")) {
        throw std::runtime_error("Error loading main theme music.");
    }
    mainTheme.setLoop(true);
    mainTheme.setVolume(50.f);
    mainTheme.play();

    fireSoundBuffer.loadFromFile("../../assets/fire.wav");
    fireSound.setBuffer(fireSoundBuffer);

    hitSoundBuffer.loadFromFile("../../assets/hit.wav");
    hitSound.setBuffer(hitSoundBuffer);

    deathSoundBuffer.loadFromFile("../../assets/death.wav");
    deathSound.setBuffer(deathSoundBuffer);

    expSoundBuffer.loadFromFile("../../assets/pickup.wav");
    expSound.setBuffer(expSoundBuffer);

    levelUpSoundBuffer.loadFromFile("../../assets/level_up.wav");
    levelUpSound.setBuffer(levelUpSoundBuffer);
}

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
        buttonTextures[i] = loadTexture(buttonTextureFiles[i]);

        levelUpButtonSprites[i].setTexture(*buttonTextures[i]);
        levelUpButtonSprites[i].setScale(5.f, 5.f);
        levelUpButtonSprites[i].setOrigin(levelUpButtonSprites[i].getLocalBounds().width / 2.f,levelUpButtonSprites[i].getLocalBounds().height);
        levelUpButtonSprites[i].setPosition(center.x + (i - 1) * buttonSpacing, center.y);

        buttonTexts[i].setFont(hudFont);
        buttonTexts[i].setString(buttonLabels[i]);
        buttonTexts[i].setCharacterSize(16);
        buttonTexts[i].setFillColor(sf::Color::White);
        buttonTexts[i].setOrigin(buttonTexts[i].getLocalBounds().width / 2.f, buttonTexts[i].getLocalBounds().height / 2.f);
        buttonTexts[i].setPosition(levelUpButtonSprites[i].getPosition().x, levelUpButtonSprites[i].getPosition().y + 10.f);
    }
}

void Game::configureUI() {
    hud.configureText(levelUpText, hudFont, "LEVEL UP!", 48, sf::Color::White, {view.getCenter().x, view.getCenter().y - view.getSize().y / 2.f + 50.f});
    configureMainMenu();
    configurePauseMenu();
    configureGameOverUI();
}

void Game::configurePauseMenu() {
    pauseBackground.setSize(sf::Vector2f(window.getSize()));
    pauseBackground.setFillColor(sf::Color(0, 0, 0, 150));

    resumeButton.setSize({200.f, 50.f});
    resumeButton.setFillColor(sf::Color::Green);
    resumeButton.setOrigin(resumeButton.getSize().x / 2.f, resumeButton.getSize().y / 2.f);
    resumeButton.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f - 50.f);

    pauseQuitButton.setSize({200.f, 50.f});
    pauseQuitButton.setFillColor(sf::Color::Red);
    pauseQuitButton.setOrigin(pauseQuitButton.getSize().x / 2.f, pauseQuitButton.getSize().y / 2.f);
    pauseQuitButton.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f + 50.f);

    hud.configureText(resumeText, hudFont, "Resume", 24, sf::Color::White, resumeButton.getPosition());
    hud.configureText(pauseQuitText, hudFont, "Quit", 24, sf::Color::White, pauseQuitButton.getPosition());
}

void Game::configureMainMenu() {
    mainMenuBackground.setSize(sf::Vector2f(window.getSize()));
    mainMenuBackground.setFillColor(sf::Color(0, 0, 0, 200)); // Semi-transparent black

    startButton.setSize({200.f, 50.f});
    startButton.setFillColor(sf::Color::Green);
    startButton.setOrigin(startButton.getSize().x / 2.f, startButton.getSize().y / 2.f);
    startButton.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f - 50.f);

    mainQuitButton.setSize({200.f, 50.f});
    mainQuitButton.setFillColor(sf::Color::Red);
    mainQuitButton.setOrigin(mainQuitButton.getSize().x / 2.f, mainQuitButton.getSize().y / 2.f);
    mainQuitButton.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f + 50.f);

    hud.configureText(startText, hudFont, "Start", 24, sf::Color::White, startButton.getPosition());
}



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

        // If the game is in the main menu, handle menu button clicks
        if (inMainMenu) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

                if (startButton.getGlobalBounds().contains(mousePos)) {
                    paused = false;
                    inMainMenu = false;  // Exit the main menu and start the game
                } else if (quitButton.getGlobalBounds().contains(mousePos)) {
                    window.close();  // Quit the game
                }
            }
            return;  // Stop further event processing in the main menu
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            paused = !paused;
        }

        if (paused) {
            mainTheme.setVolume(15.f);
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

                if (resumeButton.getGlobalBounds().contains(mousePos)) {
                    paused = false;
                } else if (pauseQuitButton.getGlobalBounds().contains(mousePos)) {
                    window.close();
                }
            }
            return;
        }
            mainTheme.setVolume(50.f);

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
                            fireballCooldownTime -= 0.1f;
                        if (fireballCooldownTime < 0.1f)
                            fireballCooldownTime = 0.1f;
                        break;
                    case 2:
                        player.shield = true;
                        break;
                    }
                    isLevelUpPaused = false;
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

    const float maxSpawnInterval = 1.5f;
    const float minSpawnInterval = 0.2f;
    const float spawnDecrementRate = 0.01f;

    enemySpawnInterval = std::max(minSpawnInterval, maxSpawnInterval - gameClock.getElapsedTime().asSeconds() * spawnDecrementRate);

    if (enemySpawnClock.getElapsedTime().asSeconds() > enemySpawnInterval) {
        if (rand() % 2 == 0) {
            spawnEnemy();
        }
        enemySpawnClock.restart();
    }
    handleCollisions();
    checkLevelUp();
}

void Game::render() {
    window.clear();

    if (inMainMenu) {
        // Render main menu
        window.draw(mainMenuBackground);
        window.draw(startButton);
        window.draw(quitButton);
        window.draw(startText);
        window.draw(mainQuitText);
    } else {
        // Your normal game rendering code here
        tilemap.draw(window);

        if (!gameOver) {
            for (const auto& exp : expPoints) exp.draw(window);
            for (const auto& enemy : enemies) enemy.draw(window);
            player.draw(window);
            if (player.shield) window.draw(shieldSprite);
            for (const auto& fireball : fireballs) fireball.draw(window);
            for (const auto& explosion : explosions) window.draw(explosion.sprite);

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

            if (finalTime >= 0) {
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
    for (auto fireballIt = fireballs.begin(); fireballIt != fireballs.end();) {
        bool fireballDestroyed = false;

        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
            if (fireballIt->getBounds().intersects(enemyIt->getBounds())) {
                sf::Vector2f expSpawnPos = enemyIt->getPosition();
                expSpawnPos.x += enemyIt->getBounds().width / 4.f;
                expSpawnPos.y += enemyIt->getBounds().height / 4.f;

                expPoints.emplace_back(expTexture, expSpawnPos.x, expSpawnPos.y);

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
                explosion.sprite.setScale(2.f, 2.f);
                explosions.push_back(explosion);
                hitSound.play();
                enemies.erase(enemyIt);
                fireballIt = fireballs.erase(fireballIt);
                fireballDestroyed = true;
                break;
            } else {
                ++enemyIt;
            }
        }
        if (!fireballDestroyed) ++fireballIt;
    }

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (player.getBounds().intersects(it->getBounds()) && !player.invincible) {
            if (!player.shield && !player.invincible) {
                if (finalTime == -1) {
                    finalTime = gameClock.getElapsedTime().asSeconds();
                }
                deathSound.play();
                gameOver = true;
                return;
            }
            if (player.shield) {
                IFrames();
                player.shield = false;
                it = enemies.erase(it);
            }
        } else {
            ++it;
        }
    }
}

void Game::checkLevelUp() {
    if (playerExp >= maxExp) {
        level++;
        playerExp = 0;
        maxExp = 5 + level * 3;
        levelUpSound.play();
        isLevelUpPaused = true;
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
    iframeClock.restart();
}

std::shared_ptr<sf::Texture> Game::loadTexture(const std::string& filepath) {
    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(filepath)) {
        throw std::runtime_error("Error loading texture: " + filepath);
    }
    return texture;
}