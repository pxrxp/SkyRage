#ifndef PLAYER_CAR_H
#define PLAYER_CAR_H

#include <SFML/Graphics.hpp>

class PlayerCar : public sf::Drawable
{
  private:
    bool leftMovement;
    bool rightMovement;
    bool accelerateMovement;

    sf::Sprite sprite;
    sf::Sprite exhaustSprite;
    sf::Vector2f positionN;

    float forwardVelocity;
    float targetVelocity;

    float minXN;
    float maxXN;

    void updatePosition(const sf::Time& deltaTime);
    void updateVelocity(const sf::Time& deltaTime);

    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const override;

    void resizeWithWindow(float newWindowWidth,
                          float newWindowHeight,
                          float initialScaleX,
                          float initialScaleY);

  public:
    PlayerCar();

    void init();
    void update(const sf::Time& deltaTime);
    void handleEvents(const sf::Event& event);

    sf::Vector2f getPositionN() const;
    float getWidthN() const;

    sf::FloatRect getBounds() const;
    bool intersectsBoundary(const sf::FloatRect& boundary);

    float getVelocity() const;
    void setVelocity(float velocity);
    void setTargetVelocity(float velocity);

    void setSpritePositionN(sf::Vector2f position);
    void setSpriteScaleN(sf::Vector2f position);
    void setMovementBoundaryN(float minX, float maxX);
};

#endif // PLAYER_CAR_H
