//
//  Component.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__Component__
#define __entitySystem__Component__

#include <iostream>
#include "Config.h"
#include <vector>
#include "assert.h"
#include <string>
#include <set>
using namespace std;
class Component
{

protected:
	Component*  _parent;
	string _id;
public:
	virtual string& GetId() {return _id;};
    Component*              GetParent() const ;
	virtual ~Component();
   
	Component(string id):_parent(NULL),_id(id){}
	void    SetParent(Component* c)  //for listening
	{
		_parent=c;
	}
    Component* operator[](string id)
    {
        return  GetC(id);
    }
	virtual void			AddC(Component* c);
	virtual void			GetCs(string id,vector<Component*>& comps/*out*/,bool recursive=false);
	virtual void			GetC(string id,Component** cmp);
	virtual Component*		GetC(string id);
    
	virtual bool			IfHasChild()const;
	virtual void			DeleteAllComponents();
    virtual Component*      Copy(Component* c)
    {
        
        c->_id=_id;
        for (Component* cc : components) {
            c->AddC(cc);
        }
        return  c;
    }
    bool                    HasTag(string& tag)
    {
       return  tags.find(tag)!=tags.end();
    }
    bool                    HasTags(set<string>& tags_)
    {
        for (auto a: tags_)
        {
           if(tags.find(a)==tags.end())
               return false;
        }
        return true;
    }

    
    void                    AddTag(const char* tag)
    {
        tags.insert(tag);
    }

	vector<Component*>   components;
    set<string>          tags;

};
#endif /* defined(__entitySystem__Component__) */
