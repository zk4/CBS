//
//  ArrayComponent.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__ArrayComponent__
#define __entitySystem__ArrayComponent__

#include "Component.h"
#include <algorithm>
class ArrayComponent :public Component
{
    vector<Component*>  package;
    const bool fixed_size;
public:
    ArrayComponent(int size,string id ):fixed_size(size!= -1),Component(id){
        if(size != -1)
        package.resize(size);
    };
    bool ExistItem(Component* c)
    {
        auto a=find_if(package.begin(), package.end(), [&c](Component* cc){return cc && c==cc;});
        return a!=package.end();
    }
    bool IsFixedSize()const
    {
        return  fixed_size;
    }
    Component* SetItem(Component* c,int i)
    {
        if(!IsFixedSize() && package.size()<i)
        {
            package.resize(i);
        }
        
        if(IsFixedSize() && i>=GetSize())return NULL;
        Component* old =GetItem(i);
        package[i]=c;
        return old;
    }
    int GetSize() const {
        return  package.size();
    }
    int FindNULL()
    {
        for (int i=0; i<GetSize(); ++i) {
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
    std::vector<Component*>::iterator
		GetBeginIte() 
    {
        return package.begin();
    }
    std::vector<Component*>::iterator
		GetEndIte()
    {
        return package.end();
    }
};

#endif /* defined(__entitySystem__ArrayComponent__) */
