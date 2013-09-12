//
//  HpComponent.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//
#pragma  once
#include "cJSON.h"
#include "Component.h"
#include "Config.h"
#include "Component.h"
#include "HpComponent.h"
#include "PackageComponent.h"
#include "HandComponent.h"
#include "AttackComponent.h"
#include "DefenseComponent.h"
#include "AllComponents.h"
class Factory 
{ 
public:
	static Component* CreateComponent(cJSON* j_entity)
	{ 
		Component* entity=NULL;
		string type = cJSON_GetObjectItem ( j_entity, "type" )->valuestring;
		cJSON* arg_array = cJSON_GetObjectItem ( j_entity, "args" );
		if("FIGHTER"==type || "SWORD"==type || "SHIELD" == type)
		{
			entity=new Component(type);
		}
		else if("HP"== type )
		{
			int hp=cJSON_GetArrayItem ( arg_array, 0 )->valueint;
			int maxhp=cJSON_GetArrayItem ( arg_array, 1 )->valueint;
			entity=new HpComponent(hp,maxhp);
		}
		else if("PACKAGE"== type )
		{
			int max_size=cJSON_GetArrayItem ( arg_array, 0 )->valueint;
			entity=new PackageComponent(max_size);
		}
		else if("DEFENSE"== type )
		{
			int defense=cJSON_GetArrayItem ( arg_array, 0 )->valueint;
			entity=new DefenseComponent(defense);
		}
		else if("HAND"== type )
		{
			entity=new HandComponent();
		}	
		else if("ATTACK"== type )
		{
			int attack=cJSON_GetArrayItem ( arg_array, 0 )->valueint;
			entity=new AttackComponent(attack);
		}
		else if("RELIVE"== type )
		{
			entity=new ReliveComponent();
		}	
		else if("SKILLPACKAGE"== type )
		{
			int size=cJSON_GetArrayItem ( arg_array, 0 )->valueint;
			entity=new PackageComponent(size,type);
		}	


		cJSON* components= cJSON_GetObjectItem ( j_entity, "components" );
		int size = cJSON_GetArraySize ( components );
		for ( int i = 0 ; i < size ; ++i ) {
			cJSON* component = cJSON_GetArrayItem ( components, i );
			entity->AddComponent(CreateComponent(component));
		}
		return entity;

	}
	static Component* CreateComponent(const char* buffer )
	{
		Component* entity=NULL;
		cJSON* root = cJSON_Parse (buffer);
		int size = cJSON_GetArraySize ( root );
		for ( int i = 0 ; i < size ; ++i ) {
			cJSON* j_entity = cJSON_GetArrayItem ( root, i );
			entity=CreateComponent(j_entity);
		}
		cJSON_Delete(root);

		return entity;
	}
};

