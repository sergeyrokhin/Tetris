#pragma once

#include <queue>
#include <memory>
#include <mutex>

#include "battlefield.h"


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
    std::shared_ptr<BattleField>(battle_field_ptr);
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