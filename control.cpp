#include "control.h"

#include <conio.h>
#include <stdio.h>
#include <thread>

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

void ControlCenter::StartInput()
{
}

void ControlCenter::InputProcess()
{
    SyncStdio sync;
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
        AddEvent(Event(EventType::GameOver));
        break;

       default:
        break;
       }
   }

}
