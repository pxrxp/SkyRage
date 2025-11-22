#include "Components/PlayerCar.h"
#include "Core/TextureManager.h"
#include "Core/WindowManager.h"
#include "States/SettingsState.h"

namespace {
static float SIDE_INCREMENT = 1.0f;
static float SIDE_SPEED_MULTIPLIER_CONST = 0.5f;

static float INITIAL_POSITION_X = 0.5f;
static float INITIAL_POSITION_Y = 0.8f;
static float INITIAL_SCALE_X = 0.05f;
static float INITIAL_SCALE_Y = 0.05f;

static float TRANSITION_DURATION = 30.0f;
}

PlayerCar::PlayerCar()
  : leftMovement(false)
  , rightMovement(false)
  , forwardVelocity(0.0f)
  , targetVelocity(0.0f)
  , positionN(sf::Vector2f(INITIAL_POSITION_X, INITIAL_POSITION_Y))
{
}

void
PlayerCar::resizeWithWindow(float newWindowWidth,
                            float newWindowHeight,
                            float initialScaleX,
                            float initialScaleY)
{
    sprite.setScale(initialScaleX * static_cast<float>(newWindowWidth) /
                      sprite.getTextureRect().getSize().x,
                    initialScaleY * static_cast<float>(newWindowHeight) /
                      sprite.getTextureRect().getSize().y);
}

float
PlayerCar::getVelocity() const
{
    return forwardVelocity;
}
void
PlayerCar::setTargetVelocity(float velocity)
{
    targetVelocity = velocity;
}

sf::Vector2f
PlayerCar::getPositionN() const
{
    return positionN;
}

void
PlayerCar::handleEvents(const sf::Event& event)
{
    if (event.type == sf::Event::Resized) {
        sprite.setPosition(positionN.x * event.size.width,
                           positionN.y * event.size.height);
        resizeWithWindow(event.size.width,
                         event.size.height,
                         INITIAL_SCALE_X,
                         INITIAL_SCALE_Y);
    }

    bool pressed = event.type != sf::Event::KeyReleased;
    leftMovement = (event.key.code == sf::Keyboard::A ||
                    event.key.code == sf::Keyboard::Left)
                     ? pressed
                     : leftMovement;
    rightMovement = (event.key.code == sf::Keyboard::D ||
                     event.key.code == sf::Keyboard::Right)
                      ? pressed
                      : rightMovement;
}

void
PlayerCar::init()
{
    auto& window = WindowManager::getWindow();
    auto& texManager = TextureManager::getInstance();
    sprite.setTexture(texManager.getTexture(TextureID::CAR0));
    sprite.setScale(INITIAL_SCALE_X * static_cast<float>(window.getSize().x) /
                      sprite.getTextureRect().getSize().x,
                    INITIAL_SCALE_Y * static_cast<float>(window.getSize().y) /
                      sprite.getTextureRect().getSize().y);

    auto spriteBounds = sprite.getLocalBounds();
    sprite.setOrigin(spriteBounds.width / 2, spriteBounds.height);
    positionN = sf::Vector2f(INITIAL_POSITION_X, INITIAL_POSITION_Y);

    float x = positionN.x * window.getSize().x;
    float y = positionN.y * window.getSize().y;
    sprite.setPosition(x, y);

    setTargetVelocity(1.0f);
    setMovementBoundaryN(0.4, 0.6);
}

void
PlayerCar::update(const sf::Time& deltaTime)
{
    updateVelocity(deltaTime);
    updatePosition(deltaTime);
}

void
PlayerCar::updateVelocity(const sf::Time& deltaTime)
{
    float velocityStep = targetVelocity - forwardVelocity;
    forwardVelocity +=
      velocityStep * deltaTime.asSeconds() / TRANSITION_DURATION;
}

void
PlayerCar::updatePosition(const sf::Time& deltaTime)
{
    auto windowSize = WindowManager::getWindow().getSize();
    float map = SettingsState::readCurrentMap();

    float dt = deltaTime.asSeconds();
    float direction = (rightMovement ? SIDE_INCREMENT : 0) -
                      (leftMovement ? SIDE_INCREMENT : 0);
    float speedMultiplier =
      static_cast<float>(windowSize.x) * SIDE_SPEED_MULTIPLIER_CONST;

    auto next_pos = sprite.getPosition() +
                    sf::Vector2f(direction * speedMultiplier * dt, 0.0f);

    if (next_pos.x > minXN * windowSize.x && next_pos.x < maxXN * windowSize.x)
        sprite.setPosition(next_pos);
}

bool
PlayerCar::intersectsBoundary(const sf::FloatRect& boundary)
{
    return boundary.intersects(sprite.getGlobalBounds());
}

float
PlayerCar::getWidthN() const
{
    auto globalBounds = sprite.getGlobalBounds();
    return globalBounds.width / WindowManager::getWindow().getSize().x;
}

void
PlayerCar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite);
}

void
PlayerCar::setSpritePositionN(sf::Vector2f position)
{
    auto windowSize = WindowManager::getWindow().getSize();
    sprite.setPosition(
      { windowSize.x * position.x, windowSize.y * position.y });
}

void
PlayerCar::setSpriteScaleN(sf::Vector2f scale)
{
    auto windowSize = WindowManager::getWindow().getSize();
    sprite.setScale({ scale.x / windowSize.x, scale.y / windowSize.y });
}

void
PlayerCar::setMovementBoundaryN(float minXN, float maxXN)
{
    this->minXN = minXN;
    this->maxXN = maxXN;
}
