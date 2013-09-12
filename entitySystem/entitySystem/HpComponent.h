//
//  HpComponent.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__HpComponent__
#define __entitySystem__HpComponent__

#include "Component.h"//

class HpComponent:public Component
{
    int _hp;
    int _maxhp;
public:
    virtual ~HpComponent(){}
    HpComponent(int hp,int maxhp,string id="HP"):_hp(hp),_maxhp(maxhp),Component(id){};
 
    int DropHp(int p){
        _hp-=p;
        return _hp ;
    }
    int GetHp()const
    {
        return _hp;
    }
    void Addhp(int p){
        _hp+=p;
        _hp=(_hp>_maxhp)?_maxhp:_hp;
    }
	void Reset()
	{
		_hp=_maxhp;
	}

    
  
};
#endif /* defined(__entitySystem__HpComponent__) */
