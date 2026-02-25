#include "States/WinState.h"
#include "Core/EventManager.h"
#include "Core/FontManager.h"
#include "Core/SoundManager.h"
#include "Core/StateManager.h"
#include "Core/TextureManager.h"
#include "Core/WindowManager.h"
#include <functional>

WinState::WinState()
  : State(StateID::Win)
{
}

void
WinState::init()
{
    auto& eventManager = EventManager::getInstance();
    auto& soundManager = SoundManager::getInstance();
    auto windowSize = WindowManager::getWindow().getSize();

    soundManager.getMusic().stop();

    overlay.setSize(sf::Vector2f(windowSize.x, windowSize.y));
    overlay.setFillColor(sf::Color(20, 30, 40, 200));  

    auto& font = FontManager::getInstance().getFont(FontID::FANCY);
    auto& textFont = FontManager::getInstance().getFont(FontID::TEXT);

    panel.setSize(sf::Vector2f(700, 450));
    panel.setFillColor(sf::Color(8, 9, 10, 250));
    panel.setOutlineThickness(1);
    panel.setOutlineColor(sf::Color(60, 70, 80));
    panel.setOrigin(panel.getSize().x / 2.0f, panel.getSize().y / 2.0f);
    panel.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    winText.setFont(font);
    winText.setString("MISSION\nACCOMPLISHED");
    winText.setCharacterSize(60);
    winText.setFillColor(sf::Color::White);
    winText.setLineSpacing(1.1f);
    auto localBounds = winText.getLocalBounds();
    winText.setOrigin(localBounds.left + localBounds.width / 2.0f,
                      localBounds.top + localBounds.height / 2.0f);
    winText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f - 40);

    instructionsText.setFont(textFont);
    instructionsText.setString("PRESS ESC TO RETURN TO HQ");
    instructionsText.setCharacterSize(28);
    instructionsText.setFillColor(sf::Color(140, 140, 140));
    auto instrBounds = instructionsText.getLocalBounds();
    instructionsText.setOrigin(instrBounds.left + instrBounds.width / 2.0f,
                               instrBounds.top + instrBounds.height / 2.0f);
    instructionsText.setPosition(windowSize.x / 2.0f,
                                 windowSize.y / 2.0f + 140);

    eventManager.addListener(
      StateID::Win,
      sf::Event::KeyPressed,
      std::bind(&WinState::handleEvents, this, std::placeholders::_1));
    eventManager.addListener(
      StateID::Win,
      sf::Event::Closed,
      std::bind(&WinState::handleEvents, this, std::placeholders::_1));
}

void
WinState::update(const sf::Time& deltaTime)
{
}

void
WinState::render(sf::RenderTarget& target)
{
    auto& snapshot = TextureManager::getInstance().getRenderTexture();
    sf::Sprite snapshotSprite(snapshot.getTexture());
    target.draw(snapshotSprite);
    target.draw(overlay);
    target.draw(panel);
    target.draw(winText);
    target.draw(instructionsText);
}

void
WinState::exit()
{
    auto& eventManager = EventManager::getInstance();
    eventManager.removeAllListeners(StateID::Win, sf::Event::KeyPressed);
    eventManager.removeAllListeners(StateID::Win, sf::Event::Closed);
}

void
WinState::handleEvents(const sf::Event& event)
{
    switch (event.type) {
        case sf::Event::KeyPressed:
            switch (event.key.code) {
                case sf::Keyboard::Escape:
                    StateManager::getInstance().popState();
                    StateManager::getInstance().popState();
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
