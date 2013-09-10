#include "Logic.h"
#include "Component.h"
#include <algorithm>

#include "HandComponent.h"
#include "AttackComponent.h"
#include "DefenseComponent.h"
#include "HpComponent.h"
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
	DropHp(defender,hp_diff);
}

int Logic::GetAttack( Component& e )
{
	 
		vector<Component*> hands;
		e.GetComponents(HAND,hands);
		int atk=0;        
		for_each(hands.begin(), hands.end(), [&atk](Component* c){
			Component* a=((HandComponent*)(c))->GetHolder();
			if(a) 
			{
				Component* p_atk=a->GetComponent(ATTACK);
				if(p_atk)
					atk+=((AttackComponent*)p_atk)->GetAttack();
			}
		}
		);
		AttackComponent* r=(AttackComponent*)e.GetComponent(ATTACK);
		if(r)
			atk+=r->GetAttack();
		return atk;
	 
}

int  Logic::GetDefense(Component& e )
{
	vector<Component*> hands;
	e.GetComponents(HAND,hands);
	int defense=0;
	for_each(hands.begin(), hands.end(), [&defense](Component* c){
		Component* a=((HandComponent*)(c))->GetHolder();
		if(a) 
		{
		Component* p_defense=a->GetComponent(DEFENSE);
		if(p_defense)
			defense+=((DefenseComponent*)p_defense)->GetDefense();
		}
	}
	);
	return defense;
}

void Logic::DropHp( Component& e,int hp )
{
	((HpComponent*)e.GetComponent(HP))->DropHp(hp);
}

void Logic::Equip( Component& holder,Component& equip,int which_hand )
{
	vector<Component* > ics;
	holder.GetComponents(HAND,ics);
	((HandComponent*)ics[which_hand])->Hold(&equip);
}

int Logic::GetHp( Component& e )
{
	return ((HpComponent*)e.GetComponent(HP))->GetHp();
}

void Logic::CastSkill( Component& caster,Component& skill, Component& suffer )
{
	int ats=GetAttack(skill);
	int def=GetDefense(suffer);
	int hp_diff=ats-def;
	DropHp(suffer,hp_diff);

}
