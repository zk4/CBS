//
//  Factory.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//
#pragma  once
#include "cJSON.h"
class Component;
class Factory 
{ 
public:
	static Component* CreateComponent(cJSON* j_entity);
	static Component* CreateComponent(const char* buffer );
};

