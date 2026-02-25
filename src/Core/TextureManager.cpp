#include "Core/TextureManager.h"
#include <cstddef>

TextureManager::TextureManager()
  : textures(static_cast<std::size_t>(TextureID::TEXTURE_COUNT))
{
}

TextureManager&
TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

sf::Texture&
TextureManager::getTexture(TextureID id)
{
    std::size_t ID = static_cast<std::size_t>(id);
    if (ID < 0 || ID >= textures.size()) {
        throw std::out_of_range("Invalid texture ID");
    }
    return textures.at(static_cast<std::size_t>(ID));
}

void
TextureManager::loadTexture(const TextureID id,
                            const std::filesystem::path& filePath)
{
    std::size_t ID = static_cast<std::size_t>(id);
    sf::Texture texture;
    texture.loadFromFile(filePath.string());
    texture.setSmooth(true);
    textures.at(ID) = std::move(texture);
}

sf::RenderTexture&
TextureManager::getRenderTexture()
{
    return renderTexture;
}
