#include "States/PlayState.h"
#include "Core/EventManager.h"
#include "Core/FontManager.h"
#include "Core/SoundManager.h"
#include "Core/StateManager.h"
#include "Core/TextureManager.h"
#include "Core/WindowManager.h"
#include "States/GameOverState.h"
#include "States/PauseState.h"
#include "States/WinState.h"
#include "Util/Path.h"
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundStream.hpp>
#include <cmath>
#include <fstream>
#include <functional>
#include <memory>

PlayState::PlayState()
  : State(StateID::Play)
  , plane()
  , background()
  , enemySpawner(0.2f, 0.8f, WindowManager::getWindow().getSize().y * 1.1f)
  , gameStarted(false)
  , countdownTimer(3.5f)
  , playingTime(0.0f)
  , distanceCovered(100000.0f)
  , gameEnded(false)
  , maxDistance(1000000.0f)
{
    auto& font = FontManager::getInstance().getFont(FontID::FANCY);
    auto& textFont = FontManager::getInstance().getFont(FontID::TEXT);
    sf::Vector2u windowSize = WindowManager::getWindow().getSize();

    timeText.setFont(textFont);
    timeText.setCharacterSize(30);
    timeText.setFillColor(sf::Color(120, 120, 120));  
    timeText.setOutlineThickness(1);
    timeText.setOutlineColor(sf::Color::Black);
    timeText.setPosition(0.05f * windowSize.x, 0.05f * windowSize.y);

    distanceText.setFont(textFont);
    distanceText.setCharacterSize(30);
    distanceText.setFillColor(sf::Color(80, 90, 100));  
    distanceText.setOutlineThickness(1);
    distanceText.setOutlineColor(sf::Color::Black);
    distanceText.setPosition(0.05f * windowSize.x, 0.09f * windowSize.y);

     
    progressBarBackground.setSize(sf::Vector2f(windowSize.x * 0.4f, 10));
    progressBarBackground.setFillColor(sf::Color(20, 20, 20, 200));
    progressBarBackground.setOutlineThickness(1);
    progressBarBackground.setOutlineColor(sf::Color(80, 80, 80));
    progressBarBackground.setPosition(windowSize.x * 0.5f,
                                      0.05f * windowSize.y);
    progressBarBackground.setOrigin(progressBarBackground.getSize().x / 2.0f,
                                    progressBarBackground.getSize().y / 2.0f);

    progressBarFill.setSize(sf::Vector2f(0, 10));
    progressBarFill.setFillColor(sf::Color(112, 128, 144));
    progressBarFill.setPosition(windowSize.x * 0.3f, 0.05f * windowSize.y);
    progressBarFill.setOrigin(0, progressBarFill.getSize().y / 2.0f);

     
    countdownText.setFont(font);
    countdownText.setCharacterSize(180);
    countdownText.setFillColor(sf::Color::White);
    countdownText.setOutlineThickness(2);
    countdownText.setOutlineColor(sf::Color(80, 90, 100));  
    countdownText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f - 100);
}

void
PlayState::init()
{
    auto& eventManager = EventManager::getInstance();
    auto& soundManager = SoundManager::getInstance();
    auto& textureManager = TextureManager::getInstance();

    textureManager.loadTexture(
      TextureID::SKY_DAY, Util::getExecutablePath() / "assets/sky_day.png");
    textureManager.loadTexture(
      TextureID::SKY_NIGHT, Util::getExecutablePath() / "assets/sky_night.png");
    textureManager.loadTexture(TextureID::SKY_SUNSET,
                               Util::getExecutablePath() /
                                 "assets/sky_evening.png");
    textureManager.loadTexture(
      TextureID::EXHAUST, Util::getExecutablePath() / "assets/exhaust.png");
    textureManager.loadTexture(TextureID::PLAYER_PLANE,
                               Util::getExecutablePath() /
                                 "assets/player_plane.png");
    textureManager.loadTexture(TextureID::ENEMY_PLANE,
                               Util::getExecutablePath() /
                                 "assets/enemy_plane.png");

    soundManager.loadSound(SoundID::THREE,
                           Util::getExecutablePath() / "assets/three.ogg");
    soundManager.loadSound(SoundID::TWO,
                           Util::getExecutablePath() / "assets/two.ogg");
    soundManager.loadSound(SoundID::ONE,
                           Util::getExecutablePath() / "assets/one.ogg");
    soundManager.loadSound(SoundID::GO,
                           Util::getExecutablePath() / "assets/go.ogg");
    soundManager.loadSound(SoundID::CRASH,
                           Util::getExecutablePath() / "assets/crash.ogg");

    auto& music = soundManager.getMusic();
    if (music.openFromFile(
          (Util::getExecutablePath() / "assets/music.ogg").string())) {
        music.setLoop(true);
        music.setVolume(20);
    }

    eventManager.addListener(
      StateID::Play,
      sf::Event::Resized,
      std::bind(&WindowManager::resizeWindow, std::placeholders::_1));

    eventManager.addListener(
      StateID::Play,
      sf::Event::KeyPressed,
      std::bind(&WindowManager::toggleFullScreen, std::placeholders::_1));

    eventManager.addListener(
      StateID::Play,
      sf::Event::Closed,
      std::bind(&PlayState::handleEvents, this, std::placeholders::_1));

    eventManager.addListener(
      StateID::Play,
      sf::Event::KeyPressed,
      std::bind(&PlayState::handleEvents, this, std::placeholders::_1));

    eventManager.addListener(
      StateID::Play,
      sf::Event::KeyPressed,
      std::bind(&PlayerPlane::handleEvents, &plane, std::placeholders::_1));

    eventManager.addListener(
      StateID::Play,
      sf::Event::KeyReleased,
      std::bind(&PlayerPlane::handleEvents, &plane, std::placeholders::_1));

    eventManager.addListener(
      StateID::Play,
      sf::Event::Resized,
      std::bind(&PlayerPlane::handleEvents, &plane, std::placeholders::_1));

    eventManager.addListener(
      StateID::Play,
      sf::Event::Resized,
      std::bind(&Background::handleEvents, &background, std::placeholders::_1));

    background.init();
    plane.init();

    playingTime = 0.0f;
    countdownTimer = 3.5f;
    gameStarted = false;
    gameEnded = false;

    soundManager.playSound(SoundID::THREE);
    soundManager.playSound(SoundID::TWO);
    soundManager.playSound(SoundID::ONE);
    soundManager.playSound(SoundID::GO);

    if (soundManager.getMusic().getStatus() != sf::SoundSource::Playing) {
        soundManager.getMusic().play();
    }
}

void
PlayState::update(const sf::Time& deltaTime)
{
    float dt = deltaTime.asSeconds();

    if (!gameStarted) {
        countdownTimer -= dt;
        if (countdownTimer <= 0.0f) {
            gameStarted = true;
            countdownTimer = 0.5f;
        } else {
            if (countdownTimer > 0.5f) {
                int val = static_cast<int>(std::ceil(countdownTimer - 0.5f));
                countdownText.setString(std::to_string(val));
                float cycle = (countdownTimer - 0.5f) - (val - 1);
                countdownText.setScale(1.0f + (1.0f - cycle) * 0.5f,
                                       1.0f + (1.0f - cycle) * 0.5f);
                countdownText.setOrigin(
                  countdownText.getLocalBounds().width / 2.0f,
                  countdownText.getLocalBounds().height / 2.0f);
            } else {
                countdownText.setString("GO!");
                countdownText.setScale(2.0f, 2.0f);
                countdownText.setOrigin(
                  countdownText.getLocalBounds().width / 2.0f,
                  countdownText.getLocalBounds().height / 2.0f);
            }
        }
        return;
    }

    if (countdownTimer > -1.0f) {
        countdownTimer -= dt;
    }

    if (gameEnded)
        return;

    plane.update(deltaTime);
    background.update(deltaTime, plane);

     
     
    float planeWidth = plane.getWidthN();
    enemySpawner.update(
      deltaTime, plane.getPositionN(), planeWidth, plane.getVelocity());

     
    playingTime += dt;
    int minutes = static_cast<int>(playingTime) / 60;
    int seconds = static_cast<int>(playingTime) % 60;

    timeText.setString("Time: " + std::to_string(minutes) + ":" +
                       (seconds < 10 ? "0" : "") + std::to_string(seconds));

     
    float speedMultiplier = 200.0f;  
    distanceCovered -= plane.getVelocity() * dt * speedMultiplier;

    if (distanceCovered <= 0.0f) {
        distanceCovered = 0.0f;
    }

    distanceText.setString(
      "Distance: " + std::to_string(static_cast<int>(distanceCovered)) + "m");

     
    float progress = 1.0f - (distanceCovered / maxDistance);
    progressBarFill.setSize(
      sf::Vector2f(progressBarBackground.getSize().x * progress, 10));

     
    auto planeBounds = plane.getBounds();
    for (auto& enemy : enemySpawner.getEnemies()) {
        if (planeBounds.intersects(enemy->getBounds())) {
            gameEnded = true;
            SoundManager::getInstance().getMusic().stop();
            SoundManager::getInstance().playSound(SoundID::CRASH);

            snapshot(TextureManager::getInstance().getRenderTexture());
            StateManager::getInstance().pushState(
              std::make_unique<GameOverState>());
            return;
        }
    }

     
    if (distanceCovered <= 0.0f) {
        gameEnded = true;
        SoundManager::getInstance().getMusic().stop();
        snapshot(TextureManager::getInstance().getRenderTexture());
        StateManager::getInstance().pushState(std::make_unique<WinState>());
        return;
    }
}

void
PlayState::render(sf::RenderTarget& target)
{
    target.draw(background);

    for (auto& enemy : enemySpawner.getEnemies()) {
        target.draw(*enemy);
    }

    target.draw(plane);

    target.draw(timeText);
    target.draw(distanceText);
    target.draw(progressBarBackground);
    target.draw(progressBarFill);

    if (countdownTimer > 0.0f) {
        target.draw(countdownText);
    }
}

void
PlayState::snapshot(sf::RenderTexture& renderTexture)
{
    auto windowSize = WindowManager::getWindow().getSize();
    renderTexture.create(windowSize.x, windowSize.y);
    render(renderTexture);
    renderTexture.display();
}

void
PlayState::handleEvents(const sf::Event& event)
{
    switch (event.type) {
        case sf::Event::KeyPressed:
            switch (event.key.code) {
                case sf::Keyboard::Escape:
                    snapshot(TextureManager::getInstance().getRenderTexture());
                    StateManager::getInstance().pushState(
                      std::make_unique<PauseState>());
                    break;
                default:
                    break;
            }
            break;
        case sf::Event::Closed:
            WindowManager::getWindow().close();
            break;
        default:
            break;
    }
}

void
PlayState::exit()
{
    auto& eventManager = EventManager::getInstance();
    SoundManager::getInstance().getMusic().stop();
    eventManager.removeAllListeners(StateID::Play, sf::Event::KeyPressed);
    eventManager.removeAllListeners(StateID::Play, sf::Event::Resized);
    eventManager.removeAllListeners(StateID::Play, sf::Event::Closed);

    if (distanceCovered <= 0.0f) {
        std::ofstream file(Util::getExecutablePath() / "highscores.txt",
                           std::ios::app);
        if (file.is_open()) {
            int minutes = static_cast<int>(playingTime) / 60;
            int seconds = static_cast<int>(playingTime) % 60;
            file << "Time: " << minutes << ":" << (seconds < 10 ? "0" : "")
                 << seconds << "\n";
            file.close();
        }
    }
}
