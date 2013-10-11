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
    PackageComponent(int size,string id ):max_size(size),Component(id){package.resize(size);};
    bool ExistItem(Component* c)
    {
        auto a=find_if(package.begin(), package.end(), [&c](Component* cc){return cc && c==cc;});
        return a!=package.end();
    }
    Component* SetItem(Component* c,int i)
    {
        if(i>=max_size)return NULL;
        Component* old =GetItem(i);
        package[i]=c;
        return old;
    }
    int GetMaxSize() const {
        return  max_size;
    }
    int FindNULL()
    {
        for (int i=0; i<max_size; ++i) {
            if(package[i]==NULL)return i;
        }
        return -1;
    }
    bool AddItem(Component* c)
    {
        int pos=FindNULL();
        if(  !ExistItem(c) && pos != -1 )
        {
            SetItem(c,pos);
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
		   auto a=find_if(package.begin(), package.end(), [&e](Component* cc){return (cc && cc->GetId()== e);});
		   if(a!=package.end())return *a;
		   else return NULL;
   }
    decltype(package.begin()) GetBeginIte() 
    {
        return package.begin();
    }
    decltype(package.end()) GetEndIte()
    {
        return package.end();
    }
};

#endif /* defined(__entitySystem__PackageComponent__) */
