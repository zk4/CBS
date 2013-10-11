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
    bool _no_max;
public:
    virtual ~ValueComponent(){}
    ValueComponent(int v,int max,bool no_max,string id):_value(v),_max(max),_no_max(no_max),Component(id){};
 
    int Minus(int p){

        return Add(-p) ;
    }
    int GetValue()const
    {
        return _value;
    }
    bool GetMax(int& max)const
    {
        if(_no_max)return  false;
        max=_max;
        return  true;
    }
    int Add(int p){
        _value+=p;
        if(!_no_max)
        _value=(_value>_max)?_max:_value;
        
        if(_value<0)_value=0;
        
        return _value;
    }
    bool AddMax(int p)
    {
        if(_no_max)return false;
        _max+=p;
        return  true;
    }
    
	bool Reset()
	{
        if(_no_max)return  false;
		_value=_max;
        return true;
	}

    
  
};
#endif /* defined(__entitySystem__HpComponent__) */
