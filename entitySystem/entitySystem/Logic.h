#pragma once
#include "Component.h"
class Component;
class Logic  
{
public:
	Logic(void);
	~Logic(void);
	static int  GetAttack(Component& e);
	static int  GetDefense(Component& e );
	static void Attack(Component& attack, Component& defender );
	static int DropHp(Component& e,int hp);
	static void Equip(Component& holder,Component& equip,int which_hand);
	static int  GetHp(Component& e);
	static void CastSkill(Component& caster,Component& skill, Component& suffer);
	static void EquipSKill(Component& holder,Component& skill);
};

