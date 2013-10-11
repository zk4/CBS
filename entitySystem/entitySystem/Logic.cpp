#include "Logic.h"
#include "Component.h"
#include <algorithm>
#include "HandComponent.h"
#include "AttackComponent.h"
#include "DefenseComponent.h"
#include "HpComponent.h"
#include "PackageComponent.h"
Logic::Logic(void)
{
}


Logic::~Logic(void)
{
}

void Logic::Attack( Component& attack, Component& defender )
{
	int ats=GetAttack(attack);
	int def=GetDefense(defender);
	int hp_diff=ats-def;
	int ret=DropHp(defender,hp_diff);
	if(ret<0)
	{ 
		PackageComponent*  p=(PackageComponent*)defender.GetComponent("SKILLPACKAGE");
		if(p)
		{
			Component* c =p->GetItem("RELIVE");
			if(c)
			{
				printf("relive\n");
				((HpComponent*)defender.GetComponent("HP"))->Reset();
			}
		}

	}

}

int Logic::GetAttack( Component& e )
{
	 
	 
	PackageComponent* hands=(PackageComponent*)e.GetComponent("HANDS");
		int atk=0;
    if(hands)
	{
		for_each(hands->GetBeginIte(), hands->GetEndIte(), [&atk](Component* c){
			 
			if(c)
			{
                vector<Component*> atks;
				c->GetComponents("ATTACK",atks,true);
				for (auto a2: atks) {
                    atk+=((AttackComponent*)a2)->GetAttack();
                }
					
			}
		}
		);
    }
		AttackComponent* r=(AttackComponent*)e.GetComponent("ATTACK");
		if(r)
			atk+=r->GetAttack();

		return atk;
	 
}

int  Logic::GetDefense(Component& e )
{
 
PackageComponent* hands=(PackageComponent*)e.GetComponent("HANDS");

	int defense=0;
    if(hands)
	{
        
        for_each(hands->GetBeginIte(), hands->GetEndIte(), [&defense](Component* c){
           
            if(c)
            {
                vector<Component*> defenses;
                 c->GetComponents("DEFENSE",defenses,true);
                for (auto a2: defenses) {
                    defense+=((DefenseComponent*)a2)->GetDefense();
                }
                
            }
        }
        );
     }
	return defense;
}

int Logic::DropHp( Component& e,int hp )
{
	return ((HpComponent*)e.GetComponent("HP"))->DropHp(hp);
}

void Logic::Equip( Component& holder,Component& equip,int which_hand )
{
	PackageComponent*  ics=(PackageComponent*)holder.GetComponent("HANDS");
	ics->SetItem(&equip,which_hand);
}

int Logic::GetHp( Component& e )
{
	return ((HpComponent*)e.GetComponent("HP"))->GetHp();
}

void Logic::CastSkill( Component& caster,Component& skill, Component& suffer )
{
	int ats=GetAttack(skill);
	int def=GetDefense(suffer);
	int hp_diff=ats-def;
	bool ret=DropHp(suffer,hp_diff);
	if(ret<0)
		{ 
			PackageComponent*  p=(PackageComponent*)suffer.GetComponent("SKILLPACKAGE");
			if(p)
				{
					Component* c =p->GetItem("RELIVE");
					if(c)
						{
							printf("relive\n");
							((HpComponent*)suffer.GetComponent("HP"))->Reset();
					}
			}
	
		}
 
}

void Logic::EquipSKill( Component& holder,Component& skill )
{
	PackageComponent* p=(PackageComponent*)holder.GetComponent("SKILLPACKAGE");
	if(p)
	{
		p->AddItem(&skill);
	}
	
}


bool Logic::TeamAdd(Component& team,Component& item)
{
    PackageComponent* p=(PackageComponent*)(team.GetComponent("SKILLPACKAGE"));
    return p->AddItem(&item);
}
