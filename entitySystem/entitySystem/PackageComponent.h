//
//  PackageComponent.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__PackageComponent__
#define __entitySystem__PackageComponent__

#include "Component.h"
#include <algorithm>
class PackageComponent :public Component
{
    vector<Component*>  package;
    const int max_size;
public:
    PackageComponent(int size,string id="PACKAGE"):max_size(size),Component(id){};
    bool ExistItem(Component* c)
    {
        auto a=find_if(package.begin(), package.end(), [&c](Component* cc){return c==cc;});
        return a!=package.end();
    }
    bool AddItem(Component* c)
    {
        if(  !ExistItem(c) && package.size()<max_size)
        {
            package.push_back(c);
            return true;
        }
        return false;
    }
   Component* GetItem(int i)
    {
        if(package.size()>i)
            return package[i];
        else
            return NULL;
    }
   Component* GetItem(string e)
   {
		   auto a=find_if(package.begin(), package.end(), [&e](Component* cc){return cc->GetId()== e;});
		   if(a!=package.end())return *a;
		   else return NULL;
	   
	    
   }
 
};

#endif /* defined(__entitySystem__PackageComponent__) */
