#pragma once

#include "battlefield.h"
#include <queue>
#include <memory>
#include <mutex>

enum class EventType {GameOver, GameStart, NextCycle, TetraminoDown, TetraminoLeft, TetraminoRight, TetraminoRotateLeft, TetraminoRotateRight, TetraminoDrop};

class Event {
public:
    EventType event_;
    Event(EventType event) : event_(event) {}
    virtual void Execute() {}
    virtual ~Event() {};
    virtual bool IsDisplaced() const noexcept {return false;}
    bool operator<(const Event & other) const noexcept {
        if (event_ >= EventType::TetraminoDown && other.event_ >= EventType::TetraminoDown)
        {
            return false;
        }
        return event_ < other.event_;
    }
};


class MyPriorityQueue : public std::priority_queue<Event> {
    public:
    void Clear() {
        c.clear();
    }
};

class ControlCenter {
    MyPriorityQueue events_;
    std::shared_ptr<Tetris> tetris_ptr_;
    std::shared_ptr<BattleField> battle_field_ptr_;

    public:
    ControlCenter(const std::shared_ptr<Tetris> & tetris_ptr, const std::shared_ptr<BattleField> battle_field_ptr) : tetris_ptr_(tetris_ptr), battle_field_ptr_(battle_field_ptr) {}
    void AddEvent(Event&& event);
    void StartInput();
    void InputProcess();
private:
    std::mutex event_mutex_;
};