#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include "Core/IState.h"

class PauseState : public State
{
  private:
    sf::RectangleShape overlay;
    sf::Text pauseText;
    sf::Text instructionsText;
    sf::Font font;

  public:
    PauseState();

    void init() override;
    void update(const sf::Time& deltaTime) override;
    void render(sf::RenderTarget& target) override;
    void handleEvents(const sf::Event& event) override;
    void exit() override;
};

#endif  
