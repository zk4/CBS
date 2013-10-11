//
//  Factory.cpp
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//
#include "Factory.h"
#include "Config.h"
#include "Component.h"
#include "Config.h"
#include "Component.h"
#include "ValueComponent.h"
#include "PackageComponent.h"

#include "AllComponents.h"
Component* Factory::CreateComponent(cJSON* j_entity)
{
    Component* entity=NULL;
    string type = cJSON_GetObjectItem ( j_entity, "type" )->valuestring;
    cJSON* arg_array = cJSON_GetObjectItem ( j_entity, "args" );
    if(type == FIGHTER || type== SWORD ||  type == SHIELD || type  == RELIVE  )
    {
        entity=new Component(type);
    }
    else if(type  == HP  || type  == DEFENSE || type  == ATTACK )
    {
        int v=cJSON_GetArrayItem ( arg_array, 0 )->valueint;
        
        int m=INT_MAX;
        if(cJSON_GetArraySize(arg_array)>1)
            m=cJSON_GetArrayItem ( arg_array, 1 )->valueint;
        
        entity=new ValueComponent(v,m,type);
    }
    else if(type  == PACKAGE )
    {
        int max_size=cJSON_GetArrayItem ( arg_array, 0 )->valueint;
        entity=new PackageComponent(max_size,type);
    }
   
    else if(type  == SKILLPACKAGE || type  == HANDS  )
    {
        int size=cJSON_GetArrayItem ( arg_array, 0 )->valueint;
        entity=new PackageComponent(size,type);
    }else
    {
        assert(0 && "type not find ");
    }
    
    
    cJSON* components= cJSON_GetObjectItem ( j_entity, "components" );
    int size = cJSON_GetArraySize ( components );
    for ( int i = 0 ; i < size ; ++i ) {
        cJSON* component = cJSON_GetArrayItem ( components, i );
        entity->AddComponent(CreateComponent(component));
    }
    return entity;
    
}
Component* Factory::CreateComponent(const char* buffer )
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