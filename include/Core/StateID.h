#ifndef STATE_ID_H
#define STATE_ID_H

#include <cstddef>

enum class StateID
{
    Default,
    Play,
    Pause,
    Menu,
    HowToPlay,
    HighScores,
    Settings,
    GameOver,
    Win
};

struct StateHash
{
    std::size_t operator()(StateID day) const
    {
        return static_cast<std::size_t>(day);
    }
};

struct StateEqual
{
    bool operator()(StateID lhs, StateID rhs) const { return lhs == rhs; }
};

#endif // STATE_ID_H
