//  Component.cpp
//  entitySystem
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.

#include "Component.h"
#include <algorithm>
#include "assert.h"
#include "ComponentMgr.h"
#include "Configuration.h"

Component* Component::AddC ( Component* c)
{
    c->SetParent (  this );
    if (GetC (c->GetName() ))assert (0);
    components[c->GetName()]=c;
    return c ;
}

bool Component::HandleMessage (const Telegram& msg)
{
    for (auto a : components)
    {
        if (a)
            a->HandleMessage (msg);

    }
    return true;
}

void Component::AutoEntityID()
{
    _ID = s_iNextValidID++;
    if (s_iNextValidID == INT_MAX)assert (0);
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
