#include "Components/EnemySpawner.h"
#include "Core/TextureManager.h"
#include "Core/WindowManager.h"
#include <random>

EnemyPlane::EnemyPlane(float x, float y)
  : lifetime(0.0f)
{
    auto& texManager = TextureManager::getInstance();
    sprite.setTexture(texManager.getTexture(TextureID::ENEMY_PLANE));

    auto& window = WindowManager::getWindow();
    float scaleX = 0.08f;
    float scaleY = 0.15f;
    sprite.setScale(scaleX * static_cast<float>(window.getSize().x) /
                      sprite.getTextureRect().getSize().x,
                    scaleY * static_cast<float>(window.getSize().y) /
                      sprite.getTextureRect().getSize().y);

    auto spriteBounds = sprite.getLocalBounds();
    sprite.setOrigin(spriteBounds.width / 2, spriteBounds.height);

    sprite.setRotation(0.f);
    sprite.setPosition(window.getSize().x * x, window.getSize().y * y);
}

sf::FloatRect
EnemyPlane::getBounds() const
{
    // Return a tightened bounding box (70% of original) for fairer collision
    // detection
    auto bounds = sprite.getGlobalBounds();
    float shrinkFactor = 0.7f;
    float newWidth = bounds.width * shrinkFactor;
    float newHeight = bounds.height * shrinkFactor;
    float offsetX = (bounds.width - newWidth) / 2.0f;
    float offsetY = (bounds.height - newHeight) / 2.0f;

    return sf::FloatRect(
      bounds.left + offsetX, bounds.top + offsetY, newWidth, newHeight);
}

void
EnemyPlane::update(const sf::Time& deltaTime, float playerVelocity)
{
    auto position = sprite.getPosition();
    auto& window = WindowManager::getWindow();

    // Base enemy speed + scale with player velocity
    float baseEnemySpeed = window.getSize().y * 0.3f;
    float relativeSpeed = playerVelocity * window.getSize().y * 0.5f;
    float totalSpeed = baseEnemySpeed + relativeSpeed;

    float newY = position.y + totalSpeed * deltaTime.asSeconds();
    sprite.setPosition(position.x, newY);

    float scaleX = 0.08f;
    float scaleY = 0.15f;

    sprite.setScale(scaleX * static_cast<float>(window.getSize().x) /
                      sprite.getTextureRect().getSize().x,
                    scaleY * static_cast<float>(window.getSize().y) /
                      sprite.getTextureRect().getSize().y);

    lifetime += deltaTime.asSeconds();
}

sf::Vector2f
EnemyPlane::getPositionPercentage() const
{
    auto& window = WindowManager::getWindow();
    auto pos = sprite.getPosition();
    return sf::Vector2f(pos.x / window.getSize().x, pos.y / window.getSize().y);
}

void
EnemyPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}

EnemySpawner::EnemySpawner(float minX, float maxX, float fixedY)
  : minX(minX)
  , maxX(maxX)
  , fixedY(fixedY)
  , spawnInterval(0.6f)
{
}

void
EnemySpawner::update(const sf::Time& deltaTime,
                     const sf::Vector2f& carPosition,
                     const float carWidthPercentage,
                     float carVelocity)
{
    if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
        spawnEnemy(carPosition, carWidthPercentage);
        spawnClock.restart();
    }

    for (auto it = enemies.begin(); it != enemies.end();) {
        (*it)->update(deltaTime, carVelocity);
        if ((*it)->getPositionPercentage().y > 1.2f) {
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}

void
EnemySpawner::spawnEnemy(const sf::Vector2f&, const float)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.05f, 0.95f);

    float x = dis(gen);
    enemies.push_back(std::make_unique<EnemyPlane>(x, 0.0f));
}

const std::deque<std::unique_ptr<EnemyPlane>>&
EnemySpawner::getEnemies() const
{
    return enemies;
}
