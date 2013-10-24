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
    
    
    public:
    T value;
    virtual ~ValueComponent(){}
    ValueComponent(T v,string id):value(v),Component(id){};
    T operator()(){
        return value;
    }
    T Minus(T p){
        
        return Add(-p) ;
    }
    T Get()const
    {
        return value;
    }
    T Add(T p){
        value+=p;
        return value;
    }
    void Set(T p){
        value=p;

    }
//    virtual Component*      Copy(Component* c)
//    {
//        Component::Copy(c);
//        
//        ValueComponent* vc=(ValueComponent*)c;
//        vc->value=value;
//
//        return  vc;
//    }

};
 #endif /* defined(__entitySystem__HpComponent__) */
