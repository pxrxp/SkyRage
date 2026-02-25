#include "Core/EventManager.h"
#include "Core/FontManager.h"
#include "Core/StateManager.h"
#include "Core/TextureManager.h"
#include "Core/WindowManager.h"
#include "Util/Path.h"
#include <fstream>

#include "States/HighScoresState.h"

HighScoresState::HighScoresState()
  : State(StateID::HighScores)
{
}

void
HighScoresState::init()
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

    panel.setSize(sf::Vector2f(600, 600));
    panel.setFillColor(sf::Color(20, 20, 20, 240));
    panel.setOutlineThickness(1);
    panel.setOutlineColor(sf::Color(112, 128, 144));
    panel.setOrigin(panel.getSize().x / 2.0f, panel.getSize().y / 2.0f);
    panel.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    title.setFont(font);
    title.setString("HALL OF FAME");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    title.setOrigin(title.getLocalBounds().width / 2.0f,
                    title.getLocalBounds().height / 2.0f);
    title.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f - 240);

    highScoresText.setFont(textFont);
    highScoresText.setCharacterSize(22);
    highScoresText.setFillColor(sf::Color(180, 180, 180));
    highScoresText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
    // Origin will be set after loading scores to center it correctly

    loadHighScores();
    eventManager.addListener(
      StateID::HighScores,
      sf::Event::KeyPressed,
      std::bind(&WindowManager::toggleFullScreen, std::placeholders::_1));
    eventManager.addListener(
      StateID::HighScores,
      sf::Event::KeyPressed,
      std::bind(&HighScoresState::handleEvents, this, std::placeholders::_1));
    eventManager.addListener(
      StateID::HighScores,
      sf::Event::Closed,
      std::bind(&HighScoresState::handleEvents, this, std::placeholders::_1));
}

void
HighScoresState::update(const sf::Time& deltaTime)
{
}

void
HighScoresState::render(sf::RenderTarget& target)
{
    target.draw(background);
    target.draw(overlay);
    target.draw(panel);
    target.draw(title);
    target.draw(highScoresText);
}

void
HighScoresState::exit()
{
    auto& eventManager = EventManager::getInstance();
    eventManager.removeAllListeners(StateID::HighScores, sf::Event::KeyPressed);
    eventManager.removeAllListeners(StateID::HighScores, sf::Event::Closed);
}

void
HighScoresState::handleEvents(const sf::Event& event)
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

void
HighScoresState::loadHighScores()
{
    std::ifstream file(Util::getExecutablePath() / "highscores.txt");
    std::string line;
    std::string scores;

    if (file.is_open()) {
        int count = 0;
        while (std::getline(file, line) && count < 10) {
            scores += std::to_string(count + 1) + ". " + line + "\n\n";
            count++;
        }
        file.close();
    } else {
        scores = "No high scores available.";
    }

    highScoresText.setString(scores);

    // Center the multi-line text block
    auto bounds = highScoresText.getLocalBounds();
    highScoresText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top);
    highScoresText.setPosition(WindowManager::getWindow().getSize().x / 2.0f,
                               WindowManager::getWindow().getSize().y / 2.0f -
                                 150);
}
