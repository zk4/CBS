//
//  Component.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__Component__
#define __entitySystem__Component__

#include <iostream>
#include "Config.h"
#include <vector>
#include "assert.h"
using namespace std;
class Component
{
    
protected:
    Component*  _parent;

    public:
    virtual eComponentId GetId()const{assert(0);return ERROR;};
	virtual bool HandleEvent(Component* c){assert(0);return false;};
    
    virtual ~Component();
    Component():_parent(NULL){}
    void    SetParent(Component* c)  //for listening
    {
        _parent=c;
    }
    virtual void   AddComponent(Component* c);
    virtual void   GetComponents(eComponentId id,vector<Component*>& comps/*out*/);
    virtual void   GetComponent(eComponentId id,Component** cmp);
    virtual Component*   GetComponent(eComponentId id);
    virtual bool IfHasChild()const;
	virtual void DeleteAllComponents();
    vector<Component*>   components;

};
#endif /* defined(__entitySystem__Component__) */
