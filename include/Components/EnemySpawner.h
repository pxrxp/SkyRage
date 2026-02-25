#ifndef ENEMYSPAWNER_H
#define ENEMYSPAWNER_H

#include <SFML/Graphics.hpp>
#include <deque>
#include <memory>

class EnemyPlane : public sf::Drawable
{
  public:
    EnemyPlane(float x, float y);
    float lifetime;
    sf::FloatRect getBounds() const;
    void update(const sf::Time& deltaTime, float playerVelocity);
    sf::Vector2f getPositionPercentage() const;

  private:
    sf::Sprite sprite;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class EnemySpawner
{
  public:
    EnemySpawner(float minX, float maxX, float fixedY);
    void update(const sf::Time& deltaTime,
                const sf::Vector2f& planePosition,
                const float planeWidthPercentage,
                float planeVelocity);
    void spawnEnemy(const sf::Vector2f& planePosition,
                    const float planeWidthPercentage);
    const std::deque<std::unique_ptr<EnemyPlane>>& getEnemies() const;

  private:
    std::deque<std::unique_ptr<EnemyPlane>> enemies;
    float minX;
    float maxX;
    float fixedY;
    sf::Clock spawnClock;
    float spawnInterval;
};

#endif  
