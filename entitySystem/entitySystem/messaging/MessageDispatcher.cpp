#include "MessageDispatcher.h"
#include "Component.h"
#include "Time/CrudeTimer.h"
#include "ComponentMgr.h"
#include "Telegram.h"
#include "stdarg.h"

#include <iostream>
using std::cout;

using std::set;


void MessageDispatcher::_Discharge (Component* pReceiver,
                                    const Telegram& telegram)
{
    if (!pReceiver->HandleMessage (telegram))
    {
        //telegram could not be handled
        cout << "Message not handled";
    }
}

//------------------------------------------------------------------------
void MessageDispatcher::DispatchMessageZZ (double  delay_micro_sec,
        int    sender,
        int    receiver,
        int    msg,
        std::initializer_list<double > args)
{
    auto pSender   = CompMgr->GetComponentFromID (sender);
    auto pReceiver = CompMgr->GetComponentFromID (receiver);
    //make sure the receiver is valid
    if (pReceiver == NULL)
    {
        cout << "\nWarning! No Receiver with ID of " << receiver << " found"<<endl<<fflush;
        return;
    }

    Telegram telegram (0, sender, receiver, msg, args);
    if (delay_micro_sec <= 0.0f)
    {
        //cout <<"sender:"<<sender<<" \nmsg "<<msg << "\nInstant telegram dispatched at time: " << Clock->GetCurrentTime()<<fflush;
        _Discharge (pReceiver, telegram);
    }
    else
    {
        long CurrentTime = Clock->GetCurrentTimes();
        telegram.DispatchTime = CurrentTime + delay_micro_sec;
        //and put it in the queue
        _PriorityQ.insert (telegram);
    }
}

void MessageDispatcher::DispatchMessageZZ (int receiver, int msg, std::initializer_list<double > list)
{
    DispatchMessageZZ (0, receiver, receiver,msg,list);
}

void  MessageDispatcher::DispatchMessageZZ (int msg, std::initializer_list<double > list)
{
    for (auto a : CompMgr->_ComponentMap)
    {
        DispatchMessageZZ (0, 0, a->GetID(), msg, list);
    }
}

void MessageDispatcher::DispatchDelayedMessages()
{
    long CurrentTime = Clock->GetCurrentTimes();
    while ( !_PriorityQ.empty() &&
            (_PriorityQ.begin()->DispatchTime < CurrentTime) &&
            (_PriorityQ.begin()->DispatchTime > 0) )
    {
        const Telegram& telegram = *_PriorityQ.begin();
        auto pReceiver = CompMgr->GetComponentFromID (telegram.Receiver);
        _Discharge (pReceiver, telegram);
        _PriorityQ.erase (_PriorityQ.begin());
    }
}

MessageDispatcher* MessageDispatcher::getMessageDispatcher()
{
    return Dispatcher;
}



