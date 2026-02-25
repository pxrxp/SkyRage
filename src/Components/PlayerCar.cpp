#include "Components/PlayerCar.h"
#include "Core/TextureManager.h"
#include "Core/WindowManager.h"
#include "States/SettingsState.h"
#include <cmath>
#include <ctime>

namespace {
static float SIDE_INCREMENT = 1.0f;
static float SIDE_SPEED_MULTIPLIER_CONST = 0.8f;

static float INITIAL_POSITION_X = 0.5f;
static float INITIAL_POSITION_Y = 0.8f;
static float INITIAL_SCALE_X = 0.08f;
static float INITIAL_SCALE_Y = 0.15f;
static float BOOST_MULTIPLIER = 2.5f;
static float MAX_VELOCITY = 8.0f; // Capped to maintain background illusion

static float TRANSITION_DURATION = 2.0f;
}

PlayerCar::PlayerCar()
  : leftMovement(false)
  , rightMovement(false)
  , accelerateMovement(false)
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
    exhaustSprite.setScale(initialScaleX * static_cast<float>(newWindowWidth) /
                             exhaustSprite.getTextureRect().getSize().x,
                           initialScaleY * static_cast<float>(newWindowHeight) /
                             exhaustSprite.getTextureRect().getSize().y);
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

    switch (event.type) {
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Left ||
                event.key.code == sf::Keyboard::A) {
                leftMovement = true;
            } else if (event.key.code == sf::Keyboard::Right ||
                       event.key.code == sf::Keyboard::D) {
                rightMovement = true;
            } else if (event.key.code == sf::Keyboard::Up ||
                       event.key.code == sf::Keyboard::W) {
                accelerateMovement = true;
            }
            break;
        case sf::Event::KeyReleased:
            if (event.key.code == sf::Keyboard::Left ||
                event.key.code == sf::Keyboard::A) {
                leftMovement = false;
            } else if (event.key.code == sf::Keyboard::Right ||
                       event.key.code == sf::Keyboard::D) {
                rightMovement = false;
            } else if (event.key.code == sf::Keyboard::Up ||
                       event.key.code == sf::Keyboard::W) {
                accelerateMovement = false;
            }
            break;
        default:
            break;
    }
}

void
PlayerCar::init()
{
    leftMovement = false;
    rightMovement = false;
    accelerateMovement = false;

    auto& texManager = TextureManager::getInstance();
    sprite.setTexture(texManager.getTexture(TextureID::PLAYER_PLANE));
    exhaustSprite.setTexture(texManager.getTexture(TextureID::EXHAUST));

    auto& window = WindowManager::getWindow();
    resizeWithWindow(
      window.getSize().x, window.getSize().y, INITIAL_SCALE_X, INITIAL_SCALE_Y);

    positionN = sf::Vector2f(INITIAL_POSITION_X, INITIAL_POSITION_Y);
    forwardVelocity = 0.0f;
    targetVelocity = 0.0f;

    sprite.setRotation(0.0f);
    exhaustSprite.setColor(sf::Color(255, 165, 0, 0));

    auto spriteBounds = sprite.getLocalBounds();
    sprite.setOrigin(spriteBounds.width / 2, spriteBounds.height);

    auto texSize = texManager.getTexture(TextureID::EXHAUST).getSize();
    // Standard full texture for exhaust (as per user update)
    exhaustSprite.setTextureRect(
      sf::IntRect(0,
                  0,
                  texManager.getTexture(TextureID::EXHAUST).getSize().x,
                  texManager.getTexture(TextureID::EXHAUST).getSize().y));
    exhaustSprite.setOrigin(exhaustSprite.getLocalBounds().width / 2.0f, 0);

    float x = positionN.x * window.getSize().x;
    float y = positionN.y * window.getSize().y;
    sprite.setPosition(x, y);

    setTargetVelocity(1.0f);
    setMovementBoundaryN(0.05f, 0.95f);
}

void
PlayerCar::update(const sf::Time& deltaTime)
{
    updateVelocity(deltaTime);
    updatePosition(deltaTime);

    if (accelerateMovement) {
        exhaustSprite.setColor(sf::Color(255, 100, 0, 200));
        float time = static_cast<float>(std::clock()) / CLOCKS_PER_SEC;
        float pulse = 1.0f + 0.5f * std::sin(time * 30.0f);
        exhaustSprite.setScale(pulse * sprite.getScale().x * 2.5f,
                               pulse * sprite.getScale().y * 2.5f);

        exhaustSprite.setPosition(sprite.getPosition().x,
                                  sprite.getPosition().y + 10.0f);
        exhaustSprite.setRotation(sprite.getRotation());
    } else {
        exhaustSprite.setColor(sf::Color(255, 100, 0, 0));
    }
}

void
PlayerCar::updateVelocity(const sf::Time& deltaTime)
{
    float dt = deltaTime.asSeconds();
    float accel = 2.0f; // Faster acceleration
    float decel = 1.0f;

    if (accelerateMovement) {
        forwardVelocity += accel * dt;
        if (forwardVelocity > MAX_VELOCITY)
            forwardVelocity = MAX_VELOCITY;
    } else {
        if (forwardVelocity > targetVelocity) {
            forwardVelocity -= decel * dt;
            if (forwardVelocity < targetVelocity)
                forwardVelocity = targetVelocity;
        } else if (forwardVelocity < targetVelocity) {
            forwardVelocity += accel * dt;
            if (forwardVelocity > targetVelocity)
                forwardVelocity = targetVelocity;
        }
    }
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

    if (next_pos.x > minXN * windowSize.x &&
        next_pos.x < maxXN * windowSize.x) {
        sprite.setPosition(next_pos);
        positionN.x = next_pos.x / windowSize.x;
    }

    // Restore banking tilt for top-down 2D view
    float targetRotation = direction * 15.0f;
    float currentRotation = sprite.getRotation();
    if (currentRotation > 180.0f)
        currentRotation -= 360.0f;

    float rotationDiff = targetRotation - currentRotation;
    float rotStep = rotationDiff * dt * 10.0f;

    sprite.setRotation(currentRotation + rotStep);
}

bool
PlayerCar::intersectsBoundary(const sf::FloatRect& boundary)
{
    return boundary.intersects(sprite.getGlobalBounds());
}

sf::FloatRect
PlayerCar::getBounds() const
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

float
PlayerCar::getWidthN() const
{
    auto globalBounds = sprite.getGlobalBounds();
    return globalBounds.width / WindowManager::getWindow().getSize().x;
}

void
PlayerCar::setVelocity(float velocity)
{
    forwardVelocity = velocity;
}

void
PlayerCar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(exhaustSprite, states);
    target.draw(sprite, states);
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
