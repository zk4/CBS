#include "MessageDispatcher.h"
#include "es/Component.h"
#include "Time/CrudeTimer.h"
#include "es/ComponentMgr.h"
#include "Telegram.h"


#include <iostream>


using std::set;


void MessageDispatcher::_Discharge (Component* pReceiver,
                                    const Telegram& telegram) {
    if (!pReceiver->HandleMessage (telegram)) {
        //telegram could not be handled
        //  printf("Message not handled");
    }
}

//------------------------------------------------------------------------
void MessageDispatcher::DispatchMessageZZ (double  delay_micro_sec,
        int    sender,
        int    receiver,
        int    msg,
        va_list ls) {
    auto pSender   = CompMgr->GetComponentFromID (sender);
    auto pReceiver = CompMgr->GetComponentFromID (receiver);
    //make sure the receiver is valid
    if (pReceiver == NULL) {
        //  printf("\nWarning! No Receiver with ID of %d found " , receiver);
        return;
    }

    static  Telegram telegram(0, sender, receiver, msg);


    telegram.Sender = sender;
    telegram.Receiver = receiver;
    telegram.Msg = msg;
    telegram.DispatchTime = 0;

    int* r = NULL;
    int i = 0;
    while (r = (int*)(va_arg(ls, void*))) {

        telegram.args[i] = r;
        ++i;
    }



    if (delay_micro_sec <= 0.0f) {
        //cout <<"sender:"<<sender<<" \nmsg "<<msg << "\nInstant telegram dispatched at time: " << Clock->GetCurrentTime()<<fflush;
        _Discharge (pReceiver, telegram);
    } else {
        long CurrentTime = Clock->GetCurrentTimes();
        telegram.DispatchTime = CurrentTime + delay_micro_sec;
        //and put it in the queue
        _PriorityQ.insert (telegram);
    }
}

void MessageDispatcher::DispatchMessageZZ(int receiver, int msg,   ...) {
    va_list ls;
    va_start(ls, msg);
    DispatchMessageZZ(0.0, receiver, receiver, msg, ls);
    va_end(ls);
}

void  MessageDispatcher::DispatchMessageZZ(int msg  , ...) {
    va_list ls;
    va_start(ls, msg);
    for (int i = CompMgr->_ComponentMap.size() - 1; i >= 0;   --i) {
        if (CompMgr->_ComponentMap[i])
            DispatchMessageZZ(0.0, 0, CompMgr->_ComponentMap[i]->GetID(), msg, ls);
    }
    va_end(ls);
}

void MessageDispatcher::DispatchDelayedMessages() {
    long CurrentTime = Clock->GetCurrentTimes();
    while ( !_PriorityQ.empty() &&
            (_PriorityQ.begin()->DispatchTime < CurrentTime) &&
            (_PriorityQ.begin()->DispatchTime > 0) ) {
        const Telegram& telegram = *_PriorityQ.begin();
        auto pReceiver = CompMgr->GetComponentFromID (telegram.Receiver);
        _Discharge (pReceiver, telegram);
        _PriorityQ.erase (_PriorityQ.begin());
    }
}

std::set<Telegram> MessageDispatcher::_PriorityQ;



