#include "Components/Background.h"
#include "Core/TextureManager.h"
#include "Core/WindowManager.h"
#include "States/SettingsState.h"

constexpr std::size_t NO_OF_ATLASES = 3;

Background::Background()
  : scrollOffset(0.0f)
  , scrollSpeed(200.0f)
  , atlases(3)
{
    currentTexture = static_cast<TextureID>(SettingsState::readCurrentMap());
}

void
Background::init()
{
    atlases.at(static_cast<std::size_t>(TextureID::SKY_DAY)) =
      Atlas(5, 11, 55, 100.0);
    atlases.at(static_cast<std::size_t>(TextureID::SKY_NIGHT)) =
      Atlas(4, 6, 23, 2.0);
    atlases.at(static_cast<std::size_t>(TextureID::SKY_SUNSET)) =
      Atlas(4, 6, 23, 500.0);

    changeAtlas(currentTexture);
}

void
Background::update(const sf::Time& deltaTime, const PlayerCar& car)
{
    float dt = deltaTime.asSeconds();

    // Scroll background downwards to simulate forward flight
    scrollOffset -= (car.getVelocity() * 160.0f + scrollSpeed) * dt;

    auto texSize = sprite.getTexture()->getSize();
    if (scrollOffset < 0.0f) {
        scrollOffset += static_cast<float>(texSize.y);
    }

    sprite.setTextureRect(
      sf::IntRect(0, static_cast<int>(scrollOffset), texSize.x, texSize.y));
}

void
Background::handleEvents(const sf::Event& event)
{
    sprite.setScale(static_cast<float>(event.size.width) /
                      sprite.getTextureRect().getSize().x,
                    static_cast<float>(event.size.height) /
                      sprite.getTextureRect().getSize().y);
}

void
Background::incrementFrame()
{
    currentFrame++;
    auto& atlas = atlases.at(static_cast<std::size_t>(currentTexture));

    if (++currentCol >= atlas.cols) {
        currentCol = 0;
        currentRow++;
    }
    if (currentFrame >= atlas.frameCount) {
        currentFrame = 0;
        currentCol = 0;
        currentRow = 0;
    }
}

void
Background::decrementFrame()
{
    currentFrame--;
    auto& atlas = atlases.at(static_cast<std::size_t>(currentTexture));

    if (--currentCol < 0) {
        currentCol = atlas.cols - 1;
        currentRow--;
    }

    if (currentFrame < 0) {
        currentFrame = atlas.frameCount - 1;
        currentCol = atlas.cols - 1;
        currentRow = atlas.rows - 1;
    }
}

void
Background::changeAtlas(TextureID textureID)
{
    currentTexture = textureID;

    auto& texManager = TextureManager::getInstance();
    auto& tex = texManager.getTexture(textureID);
    const_cast<sf::Texture&>(tex).setRepeated(
      true); // Enable repeating for scrolling
    sprite.setTexture(tex);

    auto windowSize = WindowManager::getWindow().getSize();
    sprite.setTextureRect(sf::IntRect(0, 0, tex.getSize().x, tex.getSize().y));

    sprite.setScale(static_cast<float>(windowSize.x) / tex.getSize().x,
                    static_cast<float>(windowSize.y) / tex.getSize().y);
}

void
Background::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite);
}
