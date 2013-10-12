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

 
Component* Factory::CreateComponent(cJSON* j_entity)
{
    Component* entity=NULL;
    string meta= cJSON_GetObjectItem ( j_entity, "meta" )->valuestring;
    string type = cJSON_GetObjectItem ( j_entity, "id" )->valuestring;
    cJSON* arg_array = cJSON_GetObjectItem ( j_entity, "args" );
 
     if(meta == INT)
    {
        int v=cJSON_GetArrayItem ( arg_array, 0 )->valueint;
 
        entity=new ValueComponent<int>(v,type);
    }
     else if(meta == FLOAT)
    {
        float v=cJSON_GetArrayItem ( arg_array, 0 )->valuedouble;
        
        entity=new ValueComponent<float>(v,type);
    }
    else if(meta == ARRAY)
    {
        int max_size=cJSON_GetArrayItem ( arg_array, 0 )->valueint;
        entity=new PackageComponent(max_size,type);
        
 
        int size=cJSON_GetArraySize(arg_array);
            for ( int i = 1 ; i < size ; ++i ) {
                cJSON* component = cJSON_GetArrayItem ( arg_array, i );
                ((PackageComponent*)entity)->AddItem(CreateComponent(component));
               
            }
    
    }
    else if(meta == COMPONENT)
    {
          entity=new Component(type);
//        assert(0 && "type not find ");
    }
    
    
    cJSON* components= cJSON_GetObjectItem ( j_entity, "components" );
    int size = cJSON_GetArraySize ( components );
    for ( int i = 0 ; i < size ; ++i ) {
        cJSON* component = cJSON_GetArrayItem ( components, i );
        entity->AddC(CreateComponent(component));
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