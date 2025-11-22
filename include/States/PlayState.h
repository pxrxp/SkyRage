#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include "Components/Background.h"
#include "Components/PlayerCar.h"
#include "Core/IState.h"
#include <SFML/Audio.hpp>

class PlayState : public State
{
  private:
    PlayerCar car;
    Background background;

    float distanceCovered;
    sf::Text timeText;
    sf::Text distanceText;
    sf::Clock gameClock;

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
