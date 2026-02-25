#include "States/SettingsState.h"
#include "Core/EventManager.h"
#include "Core/FontManager.h"
#include "Core/StateManager.h"
#include "Core/TextureManager.h"
#include "Core/WindowManager.h"
#include "Util/Path.h"
#include <fstream>
#include <functional>
#include <string>

std::string settingsString =
  std::string("Use arrow keys to cycle environments.\n\n") +
  std::string("* Current environment: ");

constexpr int MAX_ATLAS_ID = 2;

std::size_t
SettingsState::readCurrentMap()
{
    std::ifstream settingsFile(
      (Util::getExecutablePath() / "settings.txt").string());
    std::string line;

    if (settingsFile.is_open()) {
        if (std::getline(settingsFile, line)) {
            settingsFile.close();
            return std::clamp(std::stoi(line), 0, MAX_ATLAS_ID);
        }
        settingsFile.close();
    }
    return 0;
}

void
SettingsState::saveCurrentMap()
{
    std::ofstream settingsFile(
      (Util::getExecutablePath() / "settings.txt").string(),
      std::ios::out | std::ios::trunc);
    if (settingsFile.is_open()) {
        settingsFile << currentMap;
        settingsFile.close();
    }
}

SettingsState::SettingsState()
  : State(StateID::Settings)
{
    currentMap = readCurrentMap();
}

void
SettingsState::init()
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

    panel.setSize(sf::Vector2f(700, 450));
    panel.setFillColor(sf::Color(5, 6, 7, 250));
    panel.setOutlineThickness(1);
    panel.setOutlineColor(sf::Color(50, 60, 70));
    panel.setOrigin(panel.getSize().x / 2.0f, panel.getSize().y / 2.0f);
    panel.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    title.setFont(font);
    title.setString("MISSION CONFIG");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    title.setOrigin(
      title.getLocalBounds().left + title.getLocalBounds().width / 2.0f,
      title.getLocalBounds().top + title.getLocalBounds().height / 2.0f);
    title.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f - 140);

    settingsText.setFont(textFont);
    settingsText.setString(settingsString + std::to_string(currentMap));
    settingsText.setCharacterSize(25);
    settingsText.setFillColor(sf::Color::White);
    settingsText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f + 20);
    settingsText.setOrigin(settingsText.getLocalBounds().width / 2,
                           settingsText.getLocalBounds().height / 2);

    eventManager.addListener(
      StateID::Settings,
      sf::Event::KeyPressed,
      std::bind(&WindowManager::toggleFullScreen, std::placeholders::_1));
    eventManager.addListener(
      StateID::Settings,
      sf::Event::KeyPressed,
      std::bind(&SettingsState::handleEvents, this, std::placeholders::_1));
    eventManager.addListener(
      StateID::Settings,
      sf::Event::Closed,
      std::bind(&SettingsState::handleEvents, this, std::placeholders::_1));
}

void
SettingsState::update(const sf::Time& deltaTime)
{
    settingsText.setString(settingsString + std::to_string(currentMap));
}

void
SettingsState::render(sf::RenderTarget& target)
{
    target.draw(background);
    target.draw(overlay);
    target.draw(panel);
    target.draw(title);
    target.draw(settingsText);
}

void
SettingsState::exit()
{
    saveCurrentMap();

    auto& eventManager = EventManager::getInstance();
    eventManager.removeAllListeners(StateID::Settings, sf::Event::KeyPressed);
    eventManager.removeAllListeners(StateID::Settings, sf::Event::Closed);
}

void
SettingsState::handleEvents(const sf::Event& event)
{
    switch (event.type) {
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape) {
                StateManager::getInstance().popState();
            } else if (event.key.code == sf::Keyboard::Up) {
                currentMap = (currentMap + 1) % 3;
            } else if (event.key.code == sf::Keyboard::Down) {
                currentMap = (currentMap - 1 + 3) % 3;
            }
            break;
        case sf::Event::Closed:
            WindowManager::getWindow().close();
            break;
        default:
            break;
    }
}
