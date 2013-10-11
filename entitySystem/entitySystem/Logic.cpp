#include "Logic.h"
#include "Component.h"
#include <algorithm>

#include "ValueComponent.h"
#include "PackageComponent.h"
Logic::Logic(void)
{
}


Logic::~Logic(void)
{
}

void Logic::Attack( Component& attack, Component& defender )
{
	int atk=GetAttack(attack);
	int def=GetDefense(defender);
	int hp_diff=atk-def;
	int ret=DropHp(defender,hp_diff);
	if(ret<0)
	{ 
		PackageComponent*  p=(PackageComponent*)defender.GetComponent(SKILLPACKAGE);
		if(p)
		{
			Component* c =p->GetItem(RELIVE);
			if(c)
			{
				printf("relive\n");
				((ValueComponent*)defender.GetComponent(HP))->Reset();
			}
		}

	}

}

int Logic::GetAttack( Component& e )
{
	 
	 
	PackageComponent* hands=(PackageComponent*)e.GetComponent(HANDS);
		int atk=0;
    if(hands)
	{
		for_each(hands->GetBeginIte(), hands->GetEndIte(), [&atk](Component* c){
			 
			if(c)
			{
                vector<Component*> atks;
				c->GetComponents(ATTACK,atks,true);
				for (auto a2: atks) {
                    atk+=((ValueComponent*)a2)->GetValue();
                }
					
			}
		}
		);
    }
		ValueComponent* r=(ValueComponent*)e.GetComponent(ATTACK);
		if(r)
			atk+=r->GetValue();

		return atk;
	 
}

int  Logic::GetDefense(Component& e )
{
 
PackageComponent* hands=(PackageComponent*)e.GetComponent(HANDS);

	int defense=0;
    if(hands)
	{
        
        for_each(hands->GetBeginIte(), hands->GetEndIte(), [&defense](Component* c){
           
            if(c)
            {
                vector<Component*> defenses;
                 c->GetComponents(DEFENSE,defenses,true);
                for (auto a2: defenses) {
                    defense+=((ValueComponent*)a2)->GetValue();
                }
                
            }
        }
        );
     }
	return defense;
}

int Logic::DropHp( Component& e,int hp )
{
	return ((ValueComponent*)e.GetComponent(HP))->Minus(hp);
}

Component* Logic::Equip( Component& holder,Component& equip,int which_hand )
{
	PackageComponent*  ics=(PackageComponent*)holder.GetComponent(HANDS);
    return 	ics->SetItem(&equip,which_hand);
}

int Logic::GetHp( Component& e )
{
	return ((ValueComponent*)e.GetComponent(HP))->GetValue();
}

void Logic::CastSkill( Component& caster,Component& skill, Component& suffer )
{
	int ats=GetAttack(skill);
	int def=GetDefense(suffer);
	int hp_diff=ats-def;
	bool ret=DropHp(suffer,hp_diff);
	if(ret<0)
		{ 
			PackageComponent*  p=(PackageComponent*)suffer.GetComponent(SKILLPACKAGE);
			if(p)
				{
					Component* c =p->GetItem(RELIVE);
					if(c)
						{
							printf("relive\n");
							((ValueComponent*)suffer.GetComponent(HP))->Reset();
					}
			}
	
		}
 
}

void Logic::EquipSKill( Component& holder,Component& skill )
{
	PackageComponent* p=(PackageComponent*)holder.GetComponent(SKILLPACKAGE);
	if(p)
	{
		p->AddItem(&skill);
	}
	
}


bool Logic::TeamAdd(Component& team,Component& item)
{
    PackageComponent* p=(PackageComponent*)(team.GetComponent(SKILLPACKAGE));
    return p->AddItem(&item);
}
