#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Fireball.hpp"
#include "exp.hpp"
#include "hud.hpp"
#include "tilemap.hpp"
#include "explosion.hpp"
#include <SFML/Audio.hpp>

class Game {
public:
    Game();
    void run();

private:
    void handleEvents();
    void update(float deltaTime);
    void render();

    void spawnEnemy();
    void spawnFireball();
    void handleCollisions();
    void checkLevelUp();

    // Helper functions
    void initializeLevelUpButtons();
    std::shared_ptr<sf::Texture> loadTexture(const std::string& filepath);
    void configureGameOverUI();
    void IFrames();
    void playSound();

    sf::RenderWindow window;
    sf::View view;

    sf::Clock iframeClock;
    const float invincibilityDuration = 1.0f; // 1 second of invincibility

    sf::Sprite shieldSprite;
    std::shared_ptr<sf::Texture> shieldTexture;

    Player player;
    HUD hud;
    sf::Font hudFont; // Font for the level-up button and HUD
    TileMap tilemap;

    // Level-up handling
    bool isLevelUpPaused = false;
    sf::RectangleShape levelUpButtons[3];
    sf::Text levelUpText;
    sf::Text buttonTexts[3];
    sf::Sprite levelUpButtonSprites[3];        // Sprites for buttons
    std::shared_ptr<sf::Texture> buttonTextures[3]; // Textures for buttons

    void restartGame();

    std::shared_ptr<sf::Texture> enemyTexture;
    std::shared_ptr<sf::Texture> fireballTexture;
    std::shared_ptr<sf::Texture> expTexture;
    std::shared_ptr<sf::Texture> explosionTexture;

    struct Explosion {
        sf::Sprite sprite;
        float lifetime;
    };

    bool paused = false;
    sf::RectangleShape pauseBackground;
    sf::Text resumeText, quitText;
    sf::RectangleShape resumeButton, quitButton;

    std::vector<Explosion> explosions;
    float explosionDuration = 0.1f; // Duration in seconds

    // Game Over Text and Buttons
    sf::Text gameOverText;
    sf::Text finalTimeText;
    sf::RectangleShape restartButton;
    float finalTime = -1;
    sf::RectangleShape pauseQuitButton;
    sf::Text restartText;
    sf::Text pauseQuitText;

    sf::Music mainTheme;
    sf::SoundBuffer fireSoundBuffer;
    sf::Sound fireSound;
    sf::SoundBuffer hitSoundBuffer;
    sf::Sound hitSound;
    sf::SoundBuffer deathSoundBuffer;
    sf::Sound deathSound;
    sf::SoundBuffer expSoundBuffer;
    sf::Sound expSound;
    sf::SoundBuffer levelUpSoundBuffer;
    sf::Sound levelUpSound;

    std::vector<Enemy> enemies;
    std::vector<Fireball> fireballs;
    std::vector<Exp> expPoints;

    sf::Clock gameClock;
    sf::Clock deltaTimeClock;
    sf::Clock enemySpawnClock;
    sf::Clock fireballCooldownClock;

    int level;
    int playerExp;
    int maxExp;
    bool gameOver;

    bool isLeftMouseButtonHeld = false; // Tracks if the left mouse button is held
    float fireballCooldownTime;
    float enemySpawnInterval;
};

#endif // GAME_HPP
