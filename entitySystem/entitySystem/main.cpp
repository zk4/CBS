//
//  main.cpp
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#include <iostream>
#include "FighterComponent.h"
#include "HpComponent.h"
#include "PackageComponent.h"
#include "HandComponent.h"
#include "ItemComponent.h"
#include "AttackComponent.h"
#include "DefenseComponent.h"

int main(int argc, const char * argv[])
{
    FighterComponent* fighter=new FighterComponent();
    
    fighter->AddComponent(new HpComponent(40,40));
    fighter->AddComponent(new PackageComponent(3));
    fighter->AddComponent(new DefenseComponent(3));
    fighter->AddComponent(new HandComponent);
    fighter->AddComponent(new HandComponent);
    
     FighterComponent* fighter2=new FighterComponent();
    fighter2->AddComponent(new HpComponent(40,40));
    
    IComponent* weapon=new ItemComponent();
    weapon->AddComponent(new AttackComponent(4));
    
    IComponent* defense=new ItemComponent();
    defense->AddComponent(new DefenseComponent(5));
    
    vector<IComponent*> cps;
    

     fighter->GetComponents(HAND,cps);
 
    ((HandComponent*)cps[0])->Hold(weapon);
    ((HandComponent*)cps[1])->Hold(defense);
    cps.resize(0);
 
    fighter->Attack(fighter2);
 
    return 0;
}

