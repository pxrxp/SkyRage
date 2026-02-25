#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "Core/IState.h"
#include "Core/StateID.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>

class StateManager
{
  private:
    std::stack<std::unique_ptr<State>> states;
    StateManager() = default;

  public:
    static StateManager& getInstance();

    StateID getCurrentStateID() const;
    void pushState(std::unique_ptr<State> state);
    void popState();

    void update(const sf::Time& deltaTime);
    void render(sf::RenderTarget& target);

    StateManager& operator=(const StateManager&) = delete;
    StateManager(const StateManager&) = delete;
};
#endif  
