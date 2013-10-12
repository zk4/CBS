//
//  ValueComponent.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__HpComponent__
#define __entitySystem__HpComponent__

#include "Component.h"//

class IntComponent :public Component
{
    int _value;
    public:
    virtual ~IntComponent(){}
    IntComponent(int v,string id):_value(v),Component(id){};
    int Minus(int p){
        
        return Add(-p) ;
    }
    int Get()const
    {
        return _value;
    }
    int Add(int p){
        _value+=p;
        return _value;
    }
    void Set(int p){
        _value=p;

    }
    
};
 #endif /* defined(__entitySystem__HpComponent__) */
