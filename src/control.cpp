#include "console/consoleframe.h"
#include "control.h"

#include <stdio.h>
#include <thread>

enum class EventType
{
    GameOver,
    GameStart,
    NextCycle,
    Down,
    Left,
    Right,
    RotateLeft,
    RotateRight,
    Drop
};

bool operator<(const Event &lft, const Event &rgh)
{
    if (lft.event_ > EventType::Down && rgh.event_ > EventType::Down)
    {
        return false;
    }
    return lft.event_ < rgh.event_;
}

void ControlCenter::AddEvent(Event &&event)
{
    std::lock_guard<std::mutex> guard(event_mutex_);

    switch (event.event_)
    {
    case EventType::GameOver:
        events_.Clear();
        tetris_.Stop();
        return;
    case EventType::GameStart:
        events_.Clear();
        tetris_.Start();
        return;
    default:
        break;
    }
    events_.emplace(event);
    not_empty_.notify_all();
}

void ControlCenter::ChangeDelay(int count)
{
    auto new_delay = delay_milliseconds.load() + count * DELAY_CYCLE;
    if (new_delay < DELAY_CYCLE)
    {
        new_delay = DELAY_CYCLE;
    }
    delay_milliseconds.store(new_delay);
}

class SyncStdio
{
    bool old_status_;

public:
    SyncStdio() : old_status_(std::ios_base::sync_with_stdio(false)) {}
    ~SyncStdio() { std::ios_base::sync_with_stdio(old_status_); }
};

void ControlCenter::InputProcessThread()
{
    SyncStdio sync;
    while (true)
    {
        if (!tetris_.GameIsStart())
        {
            return;
        }
        auto ch = console::getch_my();
        switch (ch)
        {
        case 32:
            AddEvent(Event(EventType::GameOver));
            return;

        case 52:
            AddEvent(Event(EventType::Left));
            break;

        case 53:
            AddEvent(Event(EventType::Drop));
            break;

        case 57:
            AddEvent(Event(EventType::RotateRight));
            break;

        case 54:
            AddEvent(Event(EventType::Right));
            break;

        case 55:
            AddEvent(Event(EventType::RotateLeft));
            break;

        case 56:
            AddEvent(Event(EventType::Down));
            break;

        case 43:
            ChangeDelay(1);
            break;

        case 45:
            ChangeDelay(-1);
            break;

        default:
            break;
        }
    }
}

void ControlCenter::TetrisProcessThread()
{
    bool need_draw = true;
    while (tetris_.GameIsStart())
    {
        if (battle_field_ptr->NeedSent())
        {
            battle_field_ptr->SetNeedSent(false);
            tetris_.Draw();
        }

        need_draw = true;
        if (tetris_.GameIsStart())
        {
            std::unique_lock<std::mutex> lock(event_mutex_);
            while (events_.empty())
            {
                not_empty_.wait(lock);
            }

            switch (events_.top().event_)
            {
            case EventType::Down:
                tetris_.Move(MoveType::Down);
                break;
            case EventType::Left:
                tetris_.Move(MoveType::Left);
                break;
            case EventType::Right:
                tetris_.Move(MoveType::Right);
                break;
            case EventType::RotateLeft:
                tetris_.Move(MoveType::RotateL);
                break;
            case EventType::RotateRight:
                tetris_.Move(MoveType::RotateR);
                break;
            case EventType::Drop:
                tetris_.Move(MoveType::Drop);
                break;
            case EventType::GameOver:
                tetris_.Stop();
                return;
            case EventType::GameStart:
                tetris_.Start();
                break;
            default:
                break;
            }
            events_.pop();
        }
        if (need_draw)
        {
            draw(battle_field_ptr, tetris_);
            need_draw = false;
        }
        if (!tetris_.GameIsStart())
        {
            return;
        }
    }
}

void ControlCenter::TimerProcessThread()
{
    while (tetris_.GameIsStart())
    {
        std::this_thread::sleep_for(delay_milliseconds.load());
        AddEvent(Event(EventType::Down));
    }
}

ControlCenter::ControlCenter(Coordinates size) : tetris_(size), battle_field_ptr(new BattleField(size))
{
    InitScreen(size);
}

void ControlCenter::StartProceses()
{

    // console::clear_screen();
    std::cout << "Control key: Space - Exit\n"
                "4: Left; 6: Right; 9: Down\n"
                "7: RotateLeft; 9: RotateRight; 5: Drop\n"
                "+: Delay Up; -: Delay Down" << std::endl;
    console::hide_cursor();

    tetris_.Start();

    std::thread t1(&ControlCenter::TetrisProcessThread, this);
    std::thread t2(&ControlCenter::InputProcessThread, this);
    std::thread t3(&ControlCenter::TimerProcessThread, this);

    t1.join();
    t2.join();
    t3.join();
    console::enable_cursor();
}