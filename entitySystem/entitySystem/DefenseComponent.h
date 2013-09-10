//
//  DefenseComponent.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__DefenseComponent__
#define __entitySystem__DefenseComponent__

#include "IComponent.h"
class DefenseComponent:public IComponent
{
    int _defense;
public:
    virtual eComponentId GetId()const{return  DEFENSE;}
    virtual bool HandleEvent(IComponent* c)
    {
        return false;
    }
    DefenseComponent(int defense):_defense(defense){}
    int GetDefense()const
    {
        return _defense;
    }
};

#endif /* defined(__entitySystem__DefenseComponent__) */
