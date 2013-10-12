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

template<typename T>
class ValueComponent :public Component
{
    int _value;
    public:
    virtual ~ValueComponent(){}
    ValueComponent(T v,string id):_value(v),Component(id){};
    T Minus(T p){
        
        return Add(-p) ;
    }
    T Get()const
    {
        return _value;
    }
    T Add(T p){
        _value+=p;
        return _value;
    }
    void Set(T p){
        _value=p;

    }
    
};
 #endif /* defined(__entitySystem__HpComponent__) */
