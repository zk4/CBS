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
    eComponent				 _name;
    int						_ID;
    static int				s_iNextValidID;
    void AutoEntityID();
public:
    eComponent			 GetName()
    {
        return _name;
    };
    Component*    GetParent() const;
    virtual ~Component();

    Component (eComponent id);
    void    SetParent ( Component* c);


    Component* 	AddC ( Component* c);
    inline  Component* Component::GetC (eComponent name)
    {
        return components[name];
    }
    virtual bool HandleMessage (const Telegram& msg);
    int GetID() const;

    Component*   components[Component_COUNT];


};
