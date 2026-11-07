#include "States/HowToPlayState.h"
#include "Core/EventManager.h"
#include "Core/FontManager.h"
#include "Core/StateManager.h"
#include "Core/TextureManager.h"
#include "Core/WindowManager.h"
#include <functional>
#include <string>

namespace {
std::string instructionsString =
  std::string("\n\n1. Use the arrow keys or WASD to navigate your character.\n") +
  std::string("2. Avoid obstacles to progress through the game.\n") +
  std::string("3. Press ESC to pause the game.\n") +
  std::string("4. Keep an eye on the timer located in the top right corner of "
              "the screen.\n") +
  std::string(
    "5. You can switch maps from the settings menu for a new experience.") +
  std::string("\n\n* Press ESC to exit to main menu.");
}

HowToPlayState::HowToPlayState()
  : State(StateID::HowToPlay)
{
}

void
HowToPlayState::init()
{
    auto& eventManager = EventManager::getInstance();
    auto windowSize = WindowManager::getWindow().getSize();

    auto& textureManager = TextureManager::getInstance();
    background.setTexture(
      textureManager.getTexture(TextureID::MENU_BACKGROUND));
    background.setScale(
      static_cast<float>(windowSize.x) / background.getTexture()->getSize().x,
      static_cast<float>(windowSize.y) / background.getTexture()->getSize().y);

    overlay.setSize(sf::Vector2f(windowSize.x, windowSize.y));
    overlay.setFillColor(sf::Color(0, 0, 0, 230));

    auto& font = FontManager::getInstance().getFont(FontID::FANCY);
    auto& textFont = FontManager::getInstance().getFont(FontID::TEXT);

    title.setFont(font);
    title.setString("How to Play");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    title.setOrigin(
      title.getLocalBounds().left + title.getLocalBounds().width / 2.0f,
      title.getLocalBounds().top + title.getLocalBounds().height / 2.0f);
    title.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f - 140);

    instructionsText.setFont(textFont);
    instructionsText.setString(instructionsString);
    instructionsText.setCharacterSize(30);
    instructionsText.setFillColor(sf::Color::White);
    instructionsText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
    instructionsText.setOrigin(instructionsText.getLocalBounds().width / 2,
                               instructionsText.getLocalBounds().height / 2);

    eventManager.addListener(
      StateID::HowToPlay,
      sf::Event::KeyPressed,
      std::bind(&WindowManager::toggleFullScreen, std::placeholders::_1));
    eventManager.addListener(
      StateID::HowToPlay,
      sf::Event::KeyPressed,
      std::bind(&HowToPlayState::handleEvents, this, std::placeholders::_1));
    eventManager.addListener(
      StateID::HowToPlay,
      sf::Event::Closed,
      std::bind(&HowToPlayState::handleEvents, this, std::placeholders::_1));
}

void
HowToPlayState::update(const sf::Time& deltaTime)
{
}

void
HowToPlayState::render(sf::RenderTarget& target)
{
    target.draw(background);
    target.draw(overlay);
    target.draw(title);
    target.draw(instructionsText);
}

void
HowToPlayState::exit()
{
    auto& eventManager = EventManager::getInstance();
    eventManager.removeAllListeners(StateID::HowToPlay, sf::Event::KeyPressed);
    eventManager.removeAllListeners(StateID::HowToPlay, sf::Event::Closed);
}

void
HowToPlayState::handleEvents(const sf::Event& event)
{
    switch (event.type) {
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape) {
                StateManager::getInstance().popState();
            }
            break;
        case sf::Event::Closed:
            WindowManager::getWindow().close();
            break;
        default:
            break;
    }
}
