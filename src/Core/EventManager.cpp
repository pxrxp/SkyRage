#include "Core/EventManager.h"

EventManager&
EventManager::getInstance()
{
    static EventManager instance;
    return instance;
}

void
EventManager::addListener(const StateID state,
                          sf::Event::EventType eventType,
                          EventCallback callback)
{
    listeners[state][eventType].push_back(callback);
}

void
EventManager::removeAllListeners(const StateID state,
                                 sf::Event::EventType eventType)
{
    auto state_it = listeners.find(state);
    if (state_it != listeners.end()) {
        state_it->second.erase(eventType);
    }
}

void
EventManager::handleEvent(const StateID state, const sf::Event& event)
{
    auto state_it = listeners.find(state);
    if (state_it == listeners.end())
        return;

    auto event_it = state_it->second.find(event.type);
    if (event_it == state_it->second.end())
        return;

     
     
    auto callbacks_copy = event_it->second;

    for (const auto& callback : callbacks_copy) {
        callback(event);
    }
}
