#pragma  once

#include <iostream>

#include <vector>
#include "assert.h"
#include <string>
#include <set>
#include <map>
#include <memory>
#include "messaging/Telegram.h"

using namespace std;
#define  SC(x) shared_ptr<Component>(x)
class Component
{

protected:
    Component*				_parent;
    string					_name;
    int						_ID;
    static int				s_iNextValidID;
    void AutoEntityID();
public:
    virtual string&			 GetName()
    {
        return _name;
    };
    Component*    GetParent() const;
    virtual ~Component();

    Component (string id);
    void    SetParent ( Component* c);


    Component* 	AddC (shared_ptr<Component> c);

    virtual bool HandleMessage (const Telegram& msg);
    int GetID() const;

    vector<shared_ptr<Component>  >   components;


};
