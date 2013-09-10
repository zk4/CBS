//
//  HpComponent.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__HpComponent__
#define __entitySystem__HpComponent__

#include "IComponent.h"//

class HpComponent:public IComponent
{
    int _hp;
    int _maxhp;
public:
    virtual ~HpComponent(){}
    HpComponent(int hp,int maxhp):_hp(hp),_maxhp(maxhp){};
    
   eComponentId GetId()const
    {
        return HP;
    }

    bool DropHp(int p){
        _hp-=p;
        
        if(_parent)
        {
            _parent->HandleEvent(this);
        }
        return _hp>0;
    }
    int GetHp()const
    {
        return _hp;
    }
    void Addhp(int p){
        _hp+=p;
        _hp=(_hp>_maxhp)?_maxhp:_hp;
        
        if(_parent)
        {
            _parent->HandleEvent(this);
        }
    }
    
    bool HandleEvent(IComponent* c) 
    {
  
        return false;
    }
    
};
#endif /* defined(__entitySystem__HpComponent__) */
