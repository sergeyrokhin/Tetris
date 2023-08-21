#pragma once
#include <queue>
enum class EventType {GameOver, GameOver, KeyPressed, NextCycle};

class Event {
public:
    EventType event_;
    virtual void Execute() {};
    virtual ~Event() {};
    virtual bool IsDisplaced() const noexcept {return false;}
    bool operator<(const Event & other) const noexcept {
        return event_ < other.event_;
    }
};

class EventDeque {
    std::priority_queue<Event> events;
    void AddEvent(Event&& event);
};