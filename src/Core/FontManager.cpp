#include "Core/FontManager.h"

FontManager::FontManager()
  : fonts(static_cast<std::size_t>(FontID::FONT_COUNT))
{
}

FontManager&
FontManager::getInstance()
{
    static FontManager instance;
    return instance;
}

sf::Font&
FontManager::getFont(FontID id)
{
    std::size_t ID = static_cast<std::size_t>(id);
    if (ID < 0 || ID >= fonts.size()) {
        throw std::out_of_range("Invalid font ID");
    }
    return fonts.at(static_cast<std::size_t>(ID));
}

void
FontManager::loadFont(const FontID id, const std::filesystem::path& filePath)
{
    std::size_t ID = static_cast<std::size_t>(id);
    sf::Font font;
    font.loadFromFile(filePath.string());
    font.setSmooth(true);
    fonts.at(ID) = std::move(font);
}
