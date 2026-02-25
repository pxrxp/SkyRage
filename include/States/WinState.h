#ifndef WINSTATE_H
#define WINSTATE_H

#include "Core/IState.h"
#include <SFML/Graphics.hpp>

class WinState : public State
{
  private:
    sf::RectangleShape overlay;
    sf::Text winText;
    sf::Text instructionsText;
    sf::RectangleShape panel;
    sf::Font font;
    sf::Sprite backgroundSprite;

  public:
    WinState();

    void init() override;
    void update(const sf::Time& deltaTime) override;
    void render(sf::RenderTarget& target) override;
    void handleEvents(const sf::Event& event) override;
    void exit() override;
};

#endif  
