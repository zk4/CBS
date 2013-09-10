//
//  PackageComponent.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__PackageComponent__
#define __entitySystem__PackageComponent__

#include "IComponent.h"
class PackageComponent :public IComponent
{
    vector<IComponent*>  package;
    const int max_size;
public:
    PackageComponent(int size):max_size(size){};
    bool ExistItem(IComponent* c)
    {
        auto a=find_if(package.begin(), package.end(), [&c](IComponent* cc){return c==cc;});
        return a!=package.end();
    }
    bool AddItem(IComponent* c)
    {
        if(c->GetId()==ITEM && !ExistItem(c) && package.size()<max_size)
        {
            package.push_back(c);
            return true;
        }
        return false;
    }
   IComponent* GetItem(int i)
    {
        if(package.size()>i)
            return package[i];
        else
            return NULL;
    }
    eComponentId GetId()const
    {
        return  PACKAGE;
    }
    bool HandleEvent(IComponent* c)
    {
        return false;
    }
    
};

#endif /* defined(__entitySystem__PackageComponent__) */
