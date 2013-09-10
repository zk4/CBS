//
//  FighterComponent.cpp
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#include "FighterComponent.h"


eComponentId FighterComponent::GetId()const
{
    return FIGHTER;
}

bool FighterComponent::HandleEvent(IComponent* c)
{
    if(c->GetId() ==HP)
    {
        cout<<(long)this<<"now hp is:"<<((HpComponent*)GetComponent(HP))->GetHp()<<"\n";
        return true;
    }
    return false;
}