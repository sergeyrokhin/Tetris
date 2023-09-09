#pragma once

#include "battlefield.h"
#include <queue>
#include <memory>
#include <mutex>

enum class EventType;

class Event {
public:
    EventType event_;
    Event(EventType event) : event_(event) {}
};

    bool operator<(const Event & lft, const Event & rgh);


class MyPriorityQueue : public std::priority_queue<Event> {
    public:
    void Clear() {
        c.clear();
    }
};

class ControlCenter {
    Tetris tetris_;
    BattleField battle_field_;
    MyPriorityQueue events_;
    void InputProcessThread();
    void ExecProcessThread();

    public:
        ControlCenter(Coordinates size);
    void StartProceses();
    void AddEvent(Event&& event);
private:
    std::mutex event_mutex_;
};