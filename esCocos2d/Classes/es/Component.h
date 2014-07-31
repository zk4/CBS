#pragma  once

#include <iostream>

#include <vector>
#include "assert.h"

#include <set>
#include <map>
#include "Configuration.h"
#include "messaging/Telegram.h"

using namespace std;

class Component
{

protected:
    Component*				_parent;
    eComponent				_name;
    int						_ID;
    static int				s_iNextValidID;
    void AutoEntityID();
public:
    static   Component* Create (eComponent e)
    {
        return  new Component ( e);
    }
    inline  int  GetID() const
    {
        return _ID;
    }
    inline eComponent				GetName()
    {
        return _name;
    };
    inline Component*    GetParent() const
    {
        return _parent;
    }
    inline  void SetParent (Component * c)
    {
        _parent = c;
    }
    virtual ~Component();

    Component (eComponent id);

    Component* 	AddC (Component* c);
    inline  Component*  GetC (eComponent name)
    {
        return components[name];
    }

    virtual bool HandleMessage (const Telegram& msg);

    Component*   components[Component_COUNT];

};
