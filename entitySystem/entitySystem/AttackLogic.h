#pragma once
#include "Component.h"
class Component;
class AttackLogic  
{
public:
	AttackLogic(void);
	~AttackLogic(void);
	static int  GetAttack(Component& e);
	static int  GetDefense(Component& e );
	static void Attack(Component& attack, Component& defender );
	static void DropHp(Component& e,int hp);
	static void Equip(Component& holder,Component& equip,int which_hand);
	static int  GetHp(Component& e);
};

