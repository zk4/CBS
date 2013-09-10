//
//  AttackComponent.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__AttackComponent__
#define __entitySystem__AttackComponent__

#include "Component.h"
class AttackComponent:public Component
{
    int _atk;
    public :
    AttackComponent(int atk):_atk(atk){}
    eComponentId GetId()const{return ATTACK;}
     bool HandleEvent(Component* c)
    {
        return false;
    }
    int GetAttack()const {return _atk;}
};

#endif /* defined(__entitySystem__AttackComponent__) */
