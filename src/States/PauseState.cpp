#include "States/PauseState.h"
#include "Core/EventManager.h"
#include "Core/FontManager.h"
#include "Core/SoundManager.h"
#include "Core/StateManager.h"
#include "Core/TextureManager.h"
#include "Core/WindowManager.h"
#include <SFML/Graphics.hpp>
#include <functional>

PauseState::PauseState()
  : State(StateID::Pause)
{
}

void
PauseState::init()
{
    auto& eventManager = EventManager::getInstance();
    auto& soundManager = SoundManager::getInstance();
    auto windowSize = WindowManager::getWindow().getSize();

    soundManager.getMusic().pause();
    soundManager.pauseSound();

    overlay.setSize(sf::Vector2f(windowSize.x, windowSize.y));
    overlay.setFillColor(sf::Color(5, 5, 8, 235));  

    auto& font = FontManager::getInstance().getFont(FontID::FANCY);
    auto& textFont = FontManager::getInstance().getFont(FontID::TEXT);

    pauseText.setFont(font);
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(50);
    pauseText.setFillColor(sf::Color::White);
    auto localBounds = pauseText.getLocalBounds();
    pauseText.setOrigin(localBounds.width / 2.0f, localBounds.height / 2.0f);
    pauseText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    instructionsText.setFont(textFont);
    instructionsText.setString(
      "Press Esc to unpause the game\nPress Ctrl+Esc to return to main menu");
    instructionsText.setCharacterSize(28);
    instructionsText.setFillColor(sf::Color(140, 140, 140));
    instructionsText.setPosition(60.0f, windowSize.y - 120.0f);
    auto instructionsLocalBounds = instructionsText.getLocalBounds();

    eventManager.addListener(
      StateID::Pause,
      sf::Event::KeyPressed,
      std::bind(&WindowManager::toggleFullScreen, std::placeholders::_1));
    eventManager.addListener(
      StateID::Pause,
      sf::Event::KeyPressed,
      std::bind(&PauseState::handleEvents, this, std::placeholders::_1));

    eventManager.addListener(
      StateID::Pause,
      sf::Event::Closed,
      std::bind(&PauseState::handleEvents, this, std::placeholders::_1));
}

void
PauseState::update(const sf::Time& deltaTime)
{
}

void
PauseState::render(sf::RenderTarget& target)
{
    auto& snapshot = TextureManager::getInstance().getRenderTexture();
    sf::Sprite snapshotSprite(snapshot.getTexture());
    target.draw(snapshotSprite);
    target.draw(overlay);
    target.draw(pauseText);
    target.draw(instructionsText);
}

void
PauseState::exit()
{
    auto& eventManager = EventManager::getInstance();
    auto& soundManager = SoundManager::getInstance();

    eventManager.removeAllListeners(StateID::Pause, sf::Event::KeyPressed);
    eventManager.removeAllListeners(StateID::Pause, sf::Event::Closed);

    soundManager.resumeSound();
    soundManager.getMusic().play();
}

void
PauseState::handleEvents(const sf::Event& event)
{
    switch (event.type) {
        case sf::Event::KeyPressed:
            switch (event.key.code) {
                case sf::Keyboard::Escape:
                    if (event.key.control) {
                        SoundManager::getInstance().getMusic().stop();
                        StateManager::getInstance().popState();
                        StateManager::getInstance().popState();
                    } else {
                        StateManager::getInstance().popState();
                    }
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
