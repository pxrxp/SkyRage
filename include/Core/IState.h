#ifndef STATE_H
#define STATE_H

#include "Core/StateID.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include <SFML/Graphics.hpp>

class State
{
  protected:
    StateID id;

  public:
    explicit State(StateID stateId)
      : id(stateId)
    {
    }
    virtual ~State() = default;

    StateID getID() const { return id; }

    virtual void init() = 0;
    virtual void update(const sf::Time& deltaTime) = 0;
    virtual void render(sf::RenderTarget& target) = 0;
    virtual void handleEvents(const sf::Event& event) = 0;
    virtual void exit() = 0;
};

#endif  
