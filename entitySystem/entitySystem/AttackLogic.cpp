#include "AttackLogic.h"
#include "Component.h"
#include <algorithm>

#include "HandComponent.h"
#include "AttackComponent.h"
#include "DefenseComponent.h"
#include "HpComponent.h"
AttackLogic::AttackLogic(void)
{
}


AttackLogic::~AttackLogic(void)
{
}

void AttackLogic::Attack( Component& attack, Component& defender )
{
	int ats=GetAttack(attack);
	int def=GetDefense(defender);
	int hp_diff=ats-def;
	DropHp(defender,hp_diff);
}

int AttackLogic::GetAttack( Component& e )
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

int  AttackLogic::GetDefense(Component& e )
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

void AttackLogic::DropHp( Component& e,int hp )
{
	((HpComponent*)e.GetComponent(HP))->DropHp(hp);
}

void AttackLogic::Equip( Component& holder,Component& equip,int which_hand )
{
	vector<Component* > ics;
	holder.GetComponents(HAND,ics);
	((HandComponent*)ics[which_hand])->Hold(&equip);
}

int AttackLogic::GetHp( Component& e )
{
	return ((HpComponent*)e.GetComponent(HP))->GetHp();
}
