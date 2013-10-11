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
    
    static void CastSkill(Component& attack, Component& defender,string id );
    //static void CastSkill(Component& attack, Component& defender,Component& skill );
    
	static int  DropHp(Component& e,int hp);
	static Component* Equip(Component& holder,Component& equip,int which_hand);
	static int  GetHp(Component& e);
	static void CastSkill(Component& caster,Component& suffer,Component& skill );
	static void EquipSKill(Component& holder,Component& skill);
    static bool TeamAdd(Component& team,Component& item);
    static bool  IsDead(Component& c);
};

