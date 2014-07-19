//  Component.cpp
//  entitySystem
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.

#include "Component.h"
#include <algorithm>
#include "assert.h"
#include "ComponentMgr.h"
#include "Configuration.h"
Component*   Component::GetParent() const
{
    return _parent;
}

Component* Component::AddC ( Component* c)
{
    c->SetParent (  this );
    if (GetC (c->GetName() ))assert (0);
    components[c->GetName()]=c;
    return c ;
}



bool Component::HandleMessage (const Telegram& msg)
{
    return false;
}

void Component::SetParent ( Component * c) //for listening
{
    _parent = c;
}
int Component::GetID() const
{
    return _ID;
}
void Component::AutoEntityID()
{
    while (true)
    {
        s_iNextValidID++;
        if (!CompMgr->GetComponentFromID (s_iNextValidID))
        {
            _ID = s_iNextValidID;
            break;
        }
    }
    CompMgr->RegisterComponent (this);
}

Component::~Component()
{
    CompMgr->RemoveComponent (_ID);
}

Component::Component (eComponent id) :_parent (NULL), _name (id)
{
    memset (components, 0, Component_COUNT*sizeof (Component*));
    AutoEntityID();
}



int Component::s_iNextValidID=0;
