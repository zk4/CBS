//
//  Component.cpp
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#include "Component.h"
#include <algorithm>
#include "assert.h"
void Component::AddComponent(Component* c)
{
    auto a= find_if(components.begin(), components.end(), [&c](Component* cc){return cc==c;});
    if(a !=components.end())return;   //find same componet pointer
    
    c->SetParent(this);
    components.push_back(c);
}
bool Component::IfHasChild()const
{
    return components.size()>0;
}
//if direct child donesn`t has specfied id,then igorne recursive .
void Component::GetComponents(eComponentId id,vector<Component*>& cmps)
{
    for (auto a : components) {
        if(a->GetId() ==id)
            cmps.push_back(a);
    }
}

Component*   Component::GetComponent(eComponentId id)
{
    Component* i;
    GetComponent(id, &i);
    return i;
}
void  Component::GetComponent(eComponentId id,Component** cmp)
{
    vector<Component*>  cmps;
    for (auto a : components) {
        if(a->GetId() ==id)
            cmps.push_back(a);
    }
    assert(cmps.size()<=1);  //you should call  GetComponents(eComponentId id,vector<Component*>& cmps) instead
    
    if(cmps.size()==1)*cmp=cmps[0];
    else  *cmp=NULL;
    
}
