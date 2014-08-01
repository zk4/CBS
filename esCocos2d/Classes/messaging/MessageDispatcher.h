#pragma  once

#include <set>
#include <string>
#include "Singleton.h"
#include <memory>

#include "Messaging/Telegram.h"


class Component;
using namespace std;

//to make life easier...
#define Dispatcher MessageDispatcher
#define DD MessageDispatcher::DispatchMessageZZ

const double SEND_MSG_IMMEDIATELY = 0.0;
const int    NO_ADDITIONAL_INFO   = 0;
const int    RECEIVER_ALL = -1;


class MessageDispatcher
{
public:

    static std::set<Telegram> _PriorityQ;
    static void _Discharge(Component* pReceiver, const Telegram& msg);

    static   MessageDispatcher* getMessageDispatcher();
    static void DispatchDelayedMessages();
    static void DispatchMessageZZ(double delay_micro_sec, int sender, int receiver, int msg, std::initializer_list<double > list = {});
    static void DispatchMessageZZ(int receiver, int msg, std::initializer_list<double > list = {});
    //dispatch to all component
    static void DispatchMessageZZ(int msg, std::initializer_list<double > list = {});
};
