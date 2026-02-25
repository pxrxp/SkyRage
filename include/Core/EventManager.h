#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "Core/StateID.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <unordered_map>

class EventManager
{
  public:
    using EventCallback = std::function<void(const sf::Event&)>;

    static EventManager& getInstance();

    void addListener(const StateID state,
                     sf::Event::EventType eventType,
                     EventCallback callback);

    void removeAllListeners(const StateID state,
                            sf::Event::EventType eventType);
    void handleEvent(const StateID state, const sf::Event& event);

    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

  private:
    EventManager() = default;
    std::unordered_map<
      StateID,
      std::unordered_map<sf::Event::EventType, std::vector<EventCallback>>>
      listeners;
};

#endif  
