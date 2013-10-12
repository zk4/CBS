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
	vector<Component*>   components;

};
#endif /* defined(__entitySystem__Component__) */
