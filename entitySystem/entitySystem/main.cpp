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
#include "AttackLogic.h"
int main(int argc, const char * argv[])
{
    Component* fighter=new Component();
    
    fighter->AddComponent(new HpComponent(40,40));
    fighter->AddComponent(new PackageComponent(3));
    fighter->AddComponent(new DefenseComponent(3));
    fighter->AddComponent(new HandComponent);
    fighter->AddComponent(new HandComponent);
	 fighter->AddComponent(new AttackComponent(10));
   delete fighter;

     Component* fighter2=new Component();
    fighter2->AddComponent(new HpComponent(40,40));
    
    Component* weapon=new Component();
    weapon->AddComponent(new AttackComponent(4));
    
    Component* defense=new Component();
    defense->AddComponent(new DefenseComponent(5));
    
    vector<Component*> cps;
    

     fighter->GetComponents(HAND,cps);
 
 /*   ((HandComponent*)cps[0])->Hold(weapon);
    ((HandComponent*)cps[1])->Hold(defense);
    cps.resize(0);
 */
    AttackLogic::Equip(*fighter,*weapon,0);
	AttackLogic::Equip(*fighter,*defense,1);
	AttackLogic::Attack(*fighter,*fighter2);
	cout<<AttackLogic::GetHp(*fighter2)<<"\n";
	getchar();
    return 0;
}

