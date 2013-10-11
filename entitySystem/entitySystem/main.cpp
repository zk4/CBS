//
//  main.cpp
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#include <iostream>
#include "Component.h"
 
#include "PackageComponent.h"
 
 
#include "Logic.h"
#include "cJSON.h"
#include <fstream>
#include "Factory.h"
using namespace std;
int main(int argc, const char * argv[])
{


	ifstream is("component.json",  std::ios::binary  );
	if(is)
	{

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
                Component* defender=Factory::CreateComponent(cJSON_GetArrayItem ( root, 1 ));
                Component* weapon=Factory::CreateComponent(cJSON_GetArrayItem   ( root, 2 ));
                Component* defense=Factory::CreateComponent(cJSON_GetArrayItem  ( root, 3 ));
				Component* relive=Factory::CreateComponent(cJSON_GetArrayItem   ( root, 4 ));

				Logic::EquipSKill(*defender,*relive);
				Logic::Equip(*fighter,*weapon,0);
				Logic::Equip(*fighter,*defense,1);
				Logic::Attack(*fighter,*defender);
				cout<<Logic::GetHp(*defender)<<"\n";

				std::cout << "all characters read successfully.";
            }
			else
				std::cout << "error: only " << is.gcount() << " could be read";
			is.close();
			delete[] buffer;
		}


	}

	getchar();
	return 0;
}

