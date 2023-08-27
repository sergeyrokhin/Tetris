#include <conio.h>
#include <stdio.h>
#include <thread>

#include "control.h"
#include "winconsole.h"

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
        tetris_ptr_.get()->Stop();
        return;
        break;
    case EventType::GameStart :
        events_.Clear();
        tetris_ptr_.get()->Start();
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

void ControlCenter::InputProcess()
{
    //SyncStdio sync;
   while (true)
   {
       while( !_kbhit() ) {    }
       switch (_getch())
       {
       case 27:
        AddEvent(Event(EventType::GameOver));
        return;
        break;
       
       case 52:
        AddEvent(Event(EventType::Left));
        break;

       case 50:
        AddEvent(Event(EventType::Drop));
        break;

       case 53:
        AddEvent(Event(EventType::RotateRight));
        break;

       case 54:
        AddEvent(Event(EventType::Right));
        break;

       case 56:
        AddEvent(Event(EventType::RotateLeft));
        break;

       default:
        break;
       }
   }
}

void ControlCenter::ExecProcess()
{
    bool need_draw = false;
    while (true)
    {
        while ( ! events_.empty())
        {
            need_draw = true;
            std::lock_guard<std::mutex> guard(event_mutex_);
            switch (events_.top().event_)
            {
            case EventType::Down :
                tetris_ptr_.get()->Move(MoveType::Down);
                break;
            case EventType::Left :
                tetris_ptr_.get()->Move(MoveType::Left);
                break;
            case EventType::Right :
                tetris_ptr_.get()->Move(MoveType::Right);
                break;
            case EventType::RotateLeft :
                tetris_ptr_.get()->Move(MoveType::RotateL);
                break;
            case EventType::RotateRight :
                tetris_ptr_.get()->Move(MoveType::RotateR);
                break;
            case EventType::Drop :
                tetris_ptr_.get()->Move(MoveType::Drop);
                break;
            case EventType::GameOver :
                tetris_ptr_.get()->Stop();
                return;
                break;
            case EventType::GameStart :
                tetris_ptr_.get()->Start();
                break;
            default:
                break;
            }
        }
        if (need_draw)
        {
                draw(*battle_field_ptr_.get(), *tetris_ptr_.get());
                need_draw = false;
        }
    }
}

void ControlCenter::StartProcess()
{
    ConsoleFrame console;
    std::thread t1(ExecProcess);
    std::thread t2(InputProcess);
    std::thread t3(ConsoleFrame::Process, &console, std::ref(*battle_field_ptr_.get()));

    t1.join();
    t2.join();
    console.is_run_ = false;
}

// bool Event::operator<(const Event &other) const noexcept
// {
//         if (event_ >= EventType::Down && other.event_ >= EventType::Down)
//         {
//             return false;
//         }
//         return event_ < other.event_;
// }
