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

class ValueComponent:public Component
{
    int _value;
    int _max;
public:
    virtual ~ValueComponent(){}
    ValueComponent(int v,int max,string id):_value(v),_max(max),Component(id){};
 
    int Minus(int p){

        return Add(-p) ;
    }
    int GetValue()const
    {
        return _value;
    }
    int GetMax()const
    {
        return  _max;
    }
    int Add(int p){
        _value+=p;
        _value=(_value>_max)?_max:_value;
        return _value;
    }
    int AddMax(int p)
    {
        if(INT_MAX == _max)return _max;
        
        _max+=p;
        return  _max;
    }
    
	void Reset()
	{
		_value=_max;
	}

    
  
};
#endif /* defined(__entitySystem__HpComponent__) */
