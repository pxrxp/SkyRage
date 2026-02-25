#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <SFML/Graphics.hpp>
#include <filesystem>

enum class FontID
{
    TITLE,
    TEXT,
    FANCY,
    FONT_COUNT
};

class FontManager
{
  private:
    FontManager();
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
    std::vector<sf::Font> fonts;

  public:
    sf::Font& getFont(FontID id);
    void loadFont(const FontID id, const std::filesystem::path& filePath);

    static FontManager& getInstance();
};

#endif  
