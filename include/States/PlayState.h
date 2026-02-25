#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include "Components/Background.h"
#include "Components/EnemySpawner.h"
#include "Components/PlayerCar.h"
#include "Core/IState.h"
#include <SFML/Audio.hpp>

class PlayState : public State
{
  private:
    PlayerCar car;
    Background background;
    EnemySpawner enemySpawner;
    sf::RectangleShape trackLeft;
    float distanceCovered;
    float playingTime;
    float countdownTimer;
    bool gameStarted;
    sf::Text timeText;
    sf::Text distanceText;
    sf::RectangleShape progressBarBackground;
    sf::RectangleShape progressBarFill;
    sf::Text countdownText;
    float maxDistance;
    bool gameEnded;

    sf::SoundBuffer crashBuffer;
    sf::Sound crashSound;
    sf::Music bgMusic;

  public:
    PlayState();

    void init() override;
    void update(const sf::Time& deltaTime) override;
    void render(sf::RenderTarget& target) override;
    void handleEvents(const sf::Event& event) override;
    void exit() override;

    void snapshot(sf::RenderTexture& renderTexture);
};

#endif // PLAY_STATE_H
