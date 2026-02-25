#ifndef HIGHSCORESSTATE_H
#define HIGHSCORESSTATE_H

#include "Core/IState.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class HighScoresState : public State
{
  public:
    HighScoresState();
    void init() override;
    void update(const sf::Time& deltaTime) override;
    void render(sf::RenderTarget& target) override;
    void exit() override;

  private:
    void handleEvents(const sf::Event& event) override;
    void loadHighScores();

    sf::Sprite background;
    sf::RectangleShape overlay;
    sf::RectangleShape panel;
    sf::Text title;
    sf::Text highScoresText;
    std::vector<std::string> highScores;
};

#endif  
