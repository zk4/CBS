//  Component.cpp
//  entitySystem
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.

#include "Component.h"
#include <algorithm>
#include "assert.h"
Component*   Component::GetParent() const
{
    return _parent;
}

void Component::AddC(Component* c)
{
 
    c->SetParent(this);
    components[c->GetId()].insert(c);
}
bool Component::IfHasChild()const
{
    return components.size()>0;
}
//if direct child donesn`t has specfied id,then igorne recursive .
set<Component*>* Component::GetCs(const char* id)
{
    if(FindId(id))
    return &(components[id]);
    else
        return NULL;
}

bool                   Component::FindId(const char* id)
{
    return   components.find(id)!=components.end();
    
}

Component* Component::GetC(const char* id)
{
    if(FindId(id))
    {
        assert(components[id].size()==1);
        return *(components[id].begin());
    }
    else
        return NULL;
}

Component::~Component()
{
	DeleteAllComponents();
}

void Component::DeleteAllComponents()
{
	for(auto a: components)
	{
		
		for (auto d: a.second) {
            if(IfHasChild())
			{
				d->DeleteAllComponents();
			}
            delete  d;
            d=NULL;
        }
		
	}
}
