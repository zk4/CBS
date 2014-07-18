//  Component.cpp
//  entitySystem
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.

#include "Component.h"
#include <algorithm>
#include "assert.h"
#include "ComponentMgr.h"
shared_ptr<Component>   Component::GetParent() const
{
	return _parent;
}

void Component::AddC(shared_ptr<Component> c)
{
	c->SetParent(shared_ptr<Component>(this));
	components .push_back(c);
}
 
  

bool Component::HandleMessage(const Telegram& msg)
{

	return false;
}

void Component::SetParent(shared_ptr<Component> c) //for listening
{
	_parent = c;
}
int Component::GetID() const
{
	return _ID;
}
void Component::AutoEntityID()
{
	while (true){
		s_iNextValidID++;
		if (!CompMgr->FindComponentFromID(s_iNextValidID))
		{
			_ID = s_iNextValidID;
			break;
		}
	}
	CompMgr->RegisterComponent(this);
}

Component::~Component()
{
	CompMgr->RemoveComponent(  this);
}

Component::Component(string id) :_parent(shared_ptr<Component>(NULL)), _name(id)
{
	AutoEntityID();
}

int Component::s_iNextValidID=0;
