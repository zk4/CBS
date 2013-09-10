//
//  HandComponent.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__HandComponent__
#define __entitySystem__HandComponent__

#include "Component.h"
class HandComponent :public Component
{
     
    Component* _holding_item;
public:
	HandComponent():_holding_item(0){}
    eComponentId GetId()const override {return HAND;}
    Component* Hold(Component* i)
    {
       
            Component* old=_holding_item;
            _holding_item=i;
            return old;
    
        
    }
    Component* GetHolder()const
    {
        return _holding_item;
    }
    bool HandleEvent(Component* c)
    {
        return false;
    }
};
#endif /* defined(__entitySystem__HandComponent__) */
