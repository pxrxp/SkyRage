#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <SFML/Graphics.hpp>

namespace WindowManager {

sf::RenderWindow&
getWindow();

void
toggleFullScreen(const sf::Event& event);

void
resizeWindow(const sf::Event& event);

}  

#endif  
