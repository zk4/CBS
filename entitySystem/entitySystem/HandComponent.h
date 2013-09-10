//
//  HandComponent.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__HandComponent__
#define __entitySystem__HandComponent__

#include "IComponent.h"
#include "AttackComponent.h"
class HandComponent :public IComponent
{
     
    IComponent* _holding_item;
public:

    eComponentId GetId()const{return HAND;}
    IComponent* Hold(IComponent* i)
    {
        if(i->GetId()==ITEM)
        {
            IComponent* old=_holding_item;
            _holding_item=i;
            return old;
        }
        else
            return NULL;
        
    }
    IComponent* GetHolder()const
    {
        return _holding_item;
    }
    bool HandleEvent(IComponent* c)
    {
        return false;
    }
};
#endif /* defined(__entitySystem__HandComponent__) */
