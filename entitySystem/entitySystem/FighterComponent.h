//
//  FighterComponent.h
//  entitySystem
//
//  Created by ZK on 13-9-10.
//  Copyright (c) 2013年 user. All rights reserved.
//

#ifndef __entitySystem__FighterComponent__
#define __entitySystem__FighterComponent__

#include "IComponent.h"
#include "HpComponent.h"
#include "HandComponent.h"
#include "DefenseComponent.h"
class FighterComponent:public IComponent
{
    public:
    eComponentId GetId()const override;
    bool HandleEvent(IComponent* c) override;
    int  GetAttack()
    {
        vector<IComponent*> hands;
        GetComponents(HAND,hands);
        int atk=0;        
        for_each(hands.begin(), hands.end(), [&atk](IComponent* c){
            IComponent* a=((HandComponent*)(c))->GetHolder();
            IComponent* p_atk=a->GetComponent(ATTACK);
            if(p_atk)
            atk+=((AttackComponent*)p_atk)->GetAttack();
        }
      );
        return atk;
    }
    int  GetDefense()
    {
        vector<IComponent*> hands;
        GetComponents(HAND,hands);
        int atk=0;
        for_each(hands.begin(), hands.end(), [&atk](IComponent* c){
            IComponent* a=((HandComponent*)(c))->GetHolder();
            IComponent* p_atk=a->GetComponent(DEFENSE);
            if(p_atk)
                atk+=((DefenseComponent*)p_atk)->GetDefense();
        }
                 );
        return atk;
    }

    
    void Attack(IComponent* c)
    {
        int c_def=c->GetDefense();
        ((HpComponent*)c->GetComponent(HP))->DropHp(GetAttack());

    }
};

#endif /* defined(__entitySystem__FighterComponent__) */
