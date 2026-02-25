#ifndef HOWTOPLAYSTATE_H
#define HOWTOPLAYSTATE_H

#include "Core/IState.h"
#include <SFML/Graphics.hpp>

class HowToPlayState : public State
{
  public:
    HowToPlayState();
    void init() override;
    void update(const sf::Time& deltaTime) override;
    void render(sf::RenderTarget& target) override;
    void exit() override;

  private:
    void handleEvents(const sf::Event& event) override;
    sf::Sprite background;
    sf::RectangleShape overlay;
    sf::Text instructionsText;
};

#endif  
