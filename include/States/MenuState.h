#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "Core/IState.h"
#include <vector>

class MenuState : public State
{
  public:
    MenuState();
    void init() override;
    void update(const sf::Time& deltaTime) override;
    void render(sf::RenderTarget& target) override;
    void exit() override;

  private:
    void handleEvents(const sf::Event& event) override;
    void handleButtonPress(int buttonIndex);
    void updateButtonStates(const sf::Vector2f& mousePos);

    sf::Sprite background;
    sf::RectangleShape overlay;
    sf::Text title;
    std::vector<sf::Text> titleGlowLayers;
    std::vector<sf::Text> buttons;
    std::vector<sf::RectangleShape> buttonBackgrounds;
    std::vector<sf::RectangleShape> buttonBorders;
    sf::Color defaultColor;
    sf::Color hoverColor;
    int selectedButtonIndex;
};

#endif // MENUSTATE_H
