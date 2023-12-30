#pragma once

#include "battlefield.h"

#include <queue>
#include <memory>
#include <mutex>
//#include <chrono>
#include <condition_variable>

constexpr std::chrono::milliseconds DELAY_CYCLE = std::chrono::milliseconds(500);

enum class EventType;

class Event
{
public:
    EventType event_;
    Event(EventType event) : event_(event) {}
};

bool operator<(const Event &lft, const Event &rgh);

class MyPriorityQueue : public std::priority_queue<Event>
{
public:
    void Clear()
    {
        c.clear();
    }
};

class ControlCenter
{
    Tetris tetris_;
    std::shared_ptr<BattleField> battle_field_ptr;
    MyPriorityQueue events_; // Guarded event_mutex_
    void InputProcessThread();
    void TetrisProcessThread();
    void TimerProcessThread();
    void InitScreen(Coordinates size);

public:
    ControlCenter(Coordinates size);

    template <typename T>
    void AddObserver(T x)
    {
        tetris_.observer_.emplace_back(std::move(x), battle_field_ptr);
    }
    void StartProceses();
    void AddEvent(Event &&event);
    void ChangeDelay(int count);

private:
    std::mutex event_mutex_;
    std::condition_variable not_empty_;
    std::atomic<std::chrono::milliseconds> delay_milliseconds = 3 * DELAY_CYCLE;
};