//
//  IComponent.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__IComponent__
#define __entitySystem__IComponent__

#include <iostream>
#include "Config.h"
#include <vector>
using namespace std;
class IComponent
{
    
protected:
    IComponent*  _parent;

    public:
    virtual eComponentId GetId()const=0;
    virtual bool HandleEvent(IComponent* c)=0;
    
    virtual ~IComponent(){}
    IComponent():_parent(NULL){}
    void    SetParent(IComponent* c)  //for listening
    {
        _parent=c;
    }
    virtual void   AddComponent(IComponent* c);
    virtual void   GetComponents(eComponentId id,vector<IComponent*>& comps/*out*/);
    virtual void   GetComponent(eComponentId id,IComponent** cmp);
    virtual IComponent*   GetComponent(eComponentId id);
    virtual bool IfHasChild()const;
    vector<IComponent*>   components;

};
#endif /* defined(__entitySystem__IComponent__) */
