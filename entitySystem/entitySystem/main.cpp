//
//  main.cpp
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#include <iostream>
#include "Component.h"
 
#include "ArrayComponent.h"
 
 
#include "Logic.h"
#include "cJSON.h"
#include <fstream>
#include "Factory.h"

using namespace std;
int main(int argc, const char * argv[])
{
     

	ifstream is("/Users/user/GitHub/entitySystem/component.json",  std::ios::binary  );
 

		if (is) {
			// get length of file:
			is.seekg (0, is.end);
			int length = is.tellg();
			is.seekg (0, is.beg);

			char * buffer = new char [length];
			// read data as a block:
			is.read (buffer,length);

			if (is)
			{
				cJSON* root = cJSON_Parse (buffer);
				Component* fighter=Factory::CreateComponent(cJSON_GetArrayItem  ( root, 0 ));
                Component* team=Factory::CreateComponent(cJSON_GetArrayItem ( root, 1 ));
                Component* sword=Factory::CreateComponent(cJSON_GetArrayItem ( root, 2));
                 Component* synth_skill=Factory::CreateComponent(cJSON_GetArrayItem ( root,3));
                 Component* scroll1=Factory::CreateComponent(cJSON_GetArrayItem ( root, 4));
                 Component* scroll2=Factory::CreateComponent(cJSON_GetArrayItem ( root, 5));
                
              Component* skill=  Logic::SynthSkill(scroll1,scroll2,NULL);
                Logic::AddPackage(fighter, sword);
                Logic::AddHp(*fighter, 100);
                Logic::CastSkills(*fighter,*team,FIREBALL);
			 
                
				std::cout << "all characters read successfully.";
            }
			else
				std::cout << "error: only " << is.gcount() << " could be read";
			is.close();
			delete[] buffer;
		}

 
	getchar();
	return 0;
}

