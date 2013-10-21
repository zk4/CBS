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
    
    for (auto cc : components) {
        if(cc && cc==c) return;
            
    }
 
    c->SetParent(this);
    components.push_back(c);
}
bool Component::IfHasChild()const
{
    return components.size()>0;
}
//if direct child donesn`t has specfied id,then igorne recursive .
void Component::GetCs(string id,vector<Component*>& cmps,bool recursive)
{
    for (auto a : components) {
        if(a->GetId() ==id)
            cmps.push_back(a);
        if(recursive)
            a->GetCs(id, cmps,recursive);
        
    }
}

Component*   Component::GetC(string id)
{
    Component* i;
    GetC(id, &i);
    return i;
}
void  Component::GetC(string id,Component** cmp)
{
    vector<Component*>  cmps;
    for (auto a : components) {
        if(a->GetId() ==id)
            cmps.push_back(a);
    }
    assert(cmps.size()<=1);  //you should call  GetCs(eComponentId id,vector<Component*>& cmps) instead
    
    if(cmps.size()==1)*cmp=cmps[0];
    else  *cmp=NULL;
    
}

Component::~Component()
{
	DeleteAllComponents();
}

void Component::DeleteAllComponents()
{
	for(auto a: components)
	{
		if(IfHasChild())
			{
				a->DeleteAllComponents();	
			}
		delete a;
		a=NULL;
	}
}
