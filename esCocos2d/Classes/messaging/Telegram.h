#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <iostream>
#include <math.h>
#include <vector>
#include "stdarg.h"
#include <string>


class Telegram
{
public:

    int          Sender;
    int          Receiver;
    int          Msg;
    double       DispatchTime;
    std::vector<double>   args;


    Telegram() : DispatchTime (-1),
        Sender (-1),
        Receiver (-1),
        Msg (-1)
    {
    }


    Telegram (double time,
              int    sender,
              int    receiver,
              int    msg,
              std::initializer_list<double>& args_
             ) : DispatchTime (time),
        Sender (sender),
        Receiver (receiver),
        Msg (msg)

    {

        for (auto d : args_)
        {

            args.push_back (d);
        }
    }

};



const double SmallestDelay = 0.25;


inline bool operator== (const Telegram& t1, const Telegram& t2)
{
    return (fabs (t1.DispatchTime - t2.DispatchTime) < SmallestDelay) &&
           (t1.Sender == t2.Sender) &&
           (t1.Receiver == t2.Receiver) &&
           (t1.Msg == t2.Msg);
}

inline bool operator< (const Telegram& t1, const Telegram& t2)
{
    if (t1 == t2)
    {
        return false;
    }
    else
    {
        return (t1.DispatchTime < t2.DispatchTime);
    }
}

inline std::ostream& operator<< (std::ostream& os, const Telegram& t)
{
    os << "time: " << t.DispatchTime << "  Sender: " << t.Sender
       << "   Receiver: " << t.Receiver << "   Msg: " << t.Msg;
    return os;
}


template <class T>
inline T DereferenceToType (void* p)
{
    return * (T*) (p);
}


#endif