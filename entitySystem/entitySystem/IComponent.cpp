//
//  IComponent.cpp
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#include "IComponent.h"

#include "assert.h"
void IComponent::AddComponent(IComponent* c)
{
    auto a= find_if(components.begin(), components.end(), [&c](IComponent* cc){return cc==c;});
    if(a !=components.end())return;   //find same componet pointer
    
    c->SetParent(this);
    components.push_back(c);
}
bool IComponent::IfHasChild()const
{
    return components.size()>0;
}
//if direct child donesn`t has specfied id,then igorne recursive .
void IComponent::GetComponents(eComponentId id,vector<IComponent*>& cmps)
{
    for (auto a : components) {
        if(a->GetId() ==id)
            cmps.push_back(a);
    }
}

IComponent*   IComponent::GetComponent(eComponentId id)
{
    IComponent* i;
    GetComponent(id, &i);
    return i;
}
void  IComponent::GetComponent(eComponentId id,IComponent** cmp)
{
    vector<IComponent*>  cmps;
    for (auto a : components) {
        if(a->GetId() ==id)
            cmps.push_back(a);
    }
    assert(cmps.size()<=1);  //you should call  GetComponents(eComponentId id,vector<IComponent*>& cmps) instead
    
    if(cmps.size()==1)*cmp=cmps[0];
    else  *cmp=NULL;
    
}
