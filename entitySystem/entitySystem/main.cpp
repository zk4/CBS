//
//  main.cpp
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#include <iostream>
#include "Component.h"
#include "HpComponent.h"
#include "PackageComponent.h"
#include "HandComponent.h"
#include "AttackComponent.h"
#include "DefenseComponent.h"
#include "AllComponents.h"
#include "Logic.h"
int main(int argc, const char * argv[])
{
	Component* fighter=new Component(FIGHTER);

	fighter->AddComponent(new HpComponent(40,40));
	fighter->AddComponent(new PackageComponent(3));
	fighter->AddComponent(new DefenseComponent(3));
	fighter->AddComponent(new HandComponent);
	fighter->AddComponent(new HandComponent);
	fighter->AddComponent(new AttackComponent(1220));


	ReliveComponent* relive=new ReliveComponent();

	Component* defender=new Component(FIGHTER);
	defender->AddComponent(new HpComponent(40,40));
	defender->AddComponent(new PackageComponent(3,SKILLPACKAGE));
	Logic::EquipSKill(*defender,*relive);



	Component* weapon=new Component(SWORD);
	weapon->AddComponent(new AttackComponent(4));

	Component* defense=new Component(SHIELD);
	defense->AddComponent(new DefenseComponent(5));

	 

	Logic::Equip(*fighter,*weapon,0);
	Logic::Equip(*fighter,*defense,1);
	Logic::Attack(*fighter,*defender);
	cout<<Logic::GetHp(*defender)<<"\n";


	getchar();
	return 0;
}

