#pragma  once

#include <set>
#include <string>
#include "Singleton.h"
#include <memory>

#include "Messaging/Telegram.h"


class Component;
using namespace std;

//to make life easier...
#define Dispatcher MessageDispatcher::GetSingleton()
#define DD Dispatcher->DispatchMessageZZ

const double SEND_MSG_IMMEDIATELY = 0.0;
const int    NO_ADDITIONAL_INFO   = 0;
const int    SENDER_ID_IRRELEVANT = -1;


class MessageDispatcher : public Singleton<MessageDispatcher>
{
private:

    std::set<Telegram> _PriorityQ;

	void _Discharge(Component* pReceiver, const Telegram& msg);



public:
    static   MessageDispatcher* getMessageDispatcher();
    void DispatchDelayedMessages();
	void DispatchMessageZZ(double delay_micro_sec, int sender, int receiver, int msg, std::initializer_list<void*> list);
  
};
