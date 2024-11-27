#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Fireball.hpp"
#include "exp.hpp"
#include "hud.hpp"
#include "tilemap.hpp"
#include <SFML/Audio.hpp>

class Game {
public:
    Game();
    void loadTextures();
    void run();
    bool paused = true;

private:
    void handleEvents();
    void update(float deltaTime);
    void render();

    void spawnEnemy();
    void spawnFireball();
    void handleCollisions();
    void checkLevelUp();

    void initializeLevelUpButtons();
    void configureUI();
    void configurePauseMenu();
    void configureMainMenu();
    std::shared_ptr<sf::Texture> loadTexture(const std::string& filepath);
    void configureGameOverUI();
    void IFrames();
    void loadSounds();

    sf::RenderWindow window;
    sf::View view;

    sf::Clock iframeClock;
    const float invincibilityDuration = 1.0f;

    sf::Sprite shieldSprite;
    std::shared_ptr<sf::Texture> shieldTexture;

    Player player;
    HUD hud;
    sf::Font hudFont;
    TileMap tilemap;

    bool isLevelUpPaused = false;
    sf::RectangleShape levelUpButtons[3];
    sf::Text levelUpText;
    sf::Text buttonTexts[3];
    sf::Sprite levelUpButtonSprites[3];
    std::shared_ptr<sf::Texture> buttonTextures[3];

    sf::RectangleShape mainMenuBackground;
    sf::RectangleShape startButton;
    sf::RectangleShape mainQuitButton;
    sf::Text startText;
    sf::Text mainQuitText;
    bool inMainMenu = true;  // Flag to check if the game is in the main menu

    void restartGame();

    std::shared_ptr<sf::Texture> enemyTexture;
    std::shared_ptr<sf::Texture> fireballTexture;
    std::shared_ptr<sf::Texture> expTexture;
    std::shared_ptr<sf::Texture> explosionTexture;

    struct Explosion {
        sf::Sprite sprite;
        float lifetime;
    };


    sf::RectangleShape pauseBackground;
    sf::Text resumeText, quitText;
    sf::RectangleShape resumeButton, quitButton;

    std::vector<Explosion> explosions;
    float explosionDuration = 0.15f;

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

    bool isLeftMouseButtonHeld = false;
    float fireballCooldownTime;
    float enemySpawnInterval;
};

#endif
