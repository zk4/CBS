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
    static int  GetTgargetCount(Component& c);
    
    static void CastSkill(Component& attack, Component& defender,string id );
    static void CastSkill(Component& caster,Component& suffer,Component& skill );
    static void CastSkills(Component& caster,Component& suffers,string id );
    static void CastSkills(Component& caster,Component& suffers,Component& skill );
    
  	static int  AddHp(Component& e,int hp);
	static int  DropHp(Component& e,int hp);
	static Component* Equip(Component& holder,Component& equip,int which_hand);
	static int  GetHp(Component& e);
	
	static void EquipSKill(Component& holder,Component& skill);
    static bool TeamAdd(Component& team,Component& item);
    static bool  IsDead(Component& c);
};

