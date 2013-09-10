//
//  ItemComponent.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__ItemComponent__
#define __entitySystem__ItemComponent__

#include "IComponent.h"
class ItemComponent :public IComponent
{
public:
    virtual eComponentId GetId()const{return  ITEM;};
    
    virtual bool HandleEvent(IComponent* c)
    {
        return false;
    }
};
#endif /* defined(__entitySystem__ItemComponent__) */
