//
//  main.cpp
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#include <iostream>
#include "Component.h"

#include <fstream>
#include "Factory.h"
#include "Entity.h"
#include "HPComponent.h"
#include "WeaponComponent.h"
#include <memory>
#include "messaging/MessageDispatcher.h"

using namespace std;
int main (int argc, const char * argv[])
{

    auto system = Entity::Create ("System");
    auto soldier = Entity::Create ("Me");
    auto enemy = Entity::Create ("Enemy");


    soldier->AddC (HPComponent::Create (20));
    DD (0, system->GetID(), soldier->GetID(), HURT, { 22 });
    return 0;
}

