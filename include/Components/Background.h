#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Components/PlayerCar.h"
#include "Core/TextureManager.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class Background : public sf::Drawable
{
  private:
    struct Atlas
    {
        std::size_t tileWidth, tileHeight;
        std::size_t rows, cols;
        std::size_t frameCount;
        double frameDurationConstant;

        Atlas(std::size_t cols = 0,
              std::size_t rows = 0,
              std::size_t frameCount = 0,
              double frameDurationConstant = 0.0)
          : cols(cols)
          , rows(rows)
          , frameCount(frameCount)
          , tileWidth(0)
          , tileHeight(0)
          , frameDurationConstant(frameDurationConstant)
        {
        }
    };

    float scrollOffset;
    float scrollSpeed;
    sf::Sprite sprite;
    sf::Sprite secondSprite; // For seamless scrolling
    TextureID currentTexture;
    std::vector<Atlas> atlases;

    double currentTime;
    int currentFrame;
    int currentRow, currentCol;

    void incrementFrame();
    void decrementFrame();

  public:
    void init();
    void update(const sf::Time& deltaTime, const PlayerCar& car);
    void handleEvents(const sf::Event& event);

    void changeAtlas(TextureID id);

    Background();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // BACKGROUND_H
