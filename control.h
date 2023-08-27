#pragma once

#include "battlefield.h"
#include <queue>
#include <memory>
#include <mutex>

enum class EventType {GameOver, GameStart, NextCycle, Down, Left, Right, RotateLeft, RotateRight, Drop};

class Event {
public:
    EventType event_;
    Event(EventType event) : event_(event) {}
    //virtual void Execute() {}
    //virtual ~Event() {};
    //virtual bool IsDisplaced() const noexcept {return false;}
    // bool operator<(const Event & other) const noexcept {
    //     if (event_ > EventType::Down && other.event_ > EventType::Down)
    //     {
    //         return false;
    //     }
    //     return event_ < other.event_;
    // }
};

    bool operator<(const Event & lft, const Event & rgh);


class MyPriorityQueue : public std::priority_queue<Event> {
    public:
    void Clear() {
        c.clear();
    }
};

class ControlCenter {
    std::shared_ptr<Tetris> tetris_ptr_;
    std::shared_ptr<BattleField> battle_field_ptr_;
    MyPriorityQueue events_;
    void InputProcess();
    void ExecProcess();

    public:
    ControlCenter(const std::shared_ptr<Tetris> & tetris_ptr, const std::shared_ptr<BattleField> battle_field_ptr) : tetris_ptr_(tetris_ptr), battle_field_ptr_(battle_field_ptr) {}
    void StartProcess();
    void AddEvent(Event&& event);
private:
    std::mutex event_mutex_;
};