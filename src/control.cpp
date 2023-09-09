#include <conio.h>
#include <stdio.h>
#include <thread>

#include "control.h"
#include "winconsole.h"

enum class EventType { GameOver, GameStart, NextCycle, Down, Left, Right, RotateLeft, RotateRight, Drop };

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
    case EventType::GameOver :
        events_.Clear();
        tetris_.Stop();
        return;
        break;
    case EventType::GameStart :
        events_.Clear();
        tetris_.Start();
        break;
        return;
    default:
        break;
    }
    events_.emplace(event);
}

class SyncStdio {
    bool old_status_;
    public:
    SyncStdio() : old_status_(std::ios_base::sync_with_stdio(false)) {}
    ~SyncStdio() {std::ios_base::sync_with_stdio(old_status_);}
};

void ControlCenter::InputProcessThread()
{
   SyncStdio sync;
   while (true)
   {
       while( !_kbhit() ) { 
           if (!tetris_.GameIsStart())
           {
               return;
           }
       }
       auto ch = _getch();
       switch (ch)
       {
       case 27:
        AddEvent(Event(EventType::GameOver));
        return;
        break;
       
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

       default:
        break;
       }
   }
}

void ControlCenter::ExecProcessThread()
{
    //tetris_.NewTetramino();
    bool need_draw = true;
    while (tetris_.GameIsStart())
    {
        while ( ! events_.empty())
        {
            need_draw = true;
            if (tetris_.GameIsStart())
            {
                std::lock_guard<std::mutex> guard(event_mutex_);
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
                    break;
                case EventType::GameStart:
                    tetris_.Start();
                    break;
                default:
                    break;
                }
            }
            events_.pop();
        }
        if (need_draw)
        {
                draw(battle_field_, tetris_);
                need_draw = false;
        }
        if (!tetris_.GameIsStart())
        {
            return;
        }
    }
}

ControlCenter::ControlCenter(Coordinates size) : tetris_(size), battle_field_(size)
{
}

void ControlCenter::StartProceses()
{
    ConsoleFrame console;
    console.game_is_run_ = true;
    tetris_.Start();
    std::thread t1(&ControlCenter::ExecProcessThread, this);
    std::thread t2(&ControlCenter::InputProcessThread, this);
    std::thread t3(&ConsoleFrame::ConsoleProcessThread, &console, std::ref(battle_field_));

    t1.join();
    t2.join();

    console.game_is_run_ = false;
    t3.join();
}