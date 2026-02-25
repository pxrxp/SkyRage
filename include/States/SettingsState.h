#ifndef SETTINGSSTATE_H
#define SETTINGSSTATE_H

#include "Core/IState.h"
#include <SFML/Graphics.hpp>

class SettingsState : public State
{
  public:
    SettingsState();
    void init() override;
    void update(const sf::Time& deltaTime) override;
    void render(sf::RenderTarget& target) override;
    void exit() override;

    static std::size_t readCurrentMap();

  private:
    sf::Sprite background;
    sf::RectangleShape overlay;
    void handleEvents(const sf::Event& event) override;
    sf::RectangleShape panel;
    sf::Text title;
    sf::Text settingsText;

    std::size_t currentMap;

    void saveCurrentMap();
};

#endif // SETTINGSSTATE_H
