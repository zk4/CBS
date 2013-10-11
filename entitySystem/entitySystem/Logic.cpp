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
void Logic::CastSkill(Component& attack, Component& defender,string id )
{
  	PackageComponent*  p=(PackageComponent*)attack.GetComponent(SKILLPACKAGE);
    if(p)
    {
        Component* c =p->GetItem(id);
        if(c)
        {
            
            CastSkill(attack,defender,*c);
        }
    }
}
void Logic::CastSkills(Component& caster,Component& suffers,string id )
{
    PackageComponent*  p=(PackageComponent*)caster.GetComponent(SKILLPACKAGE);
    if(p)
    {
        Component* c =p->GetItem(id);
        if(c)
        {
            
            CastSkills(caster,suffers,*c);
        }
    }
}
void Logic::Attack( Component& attack, Component& defender )
{
	int atk=GetAttack(attack);
	int def=GetDefense(defender);
	int hp_diff=atk-def;
    printf("drop hp:%d\n",hp_diff);
    DropHp(defender,hp_diff);
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
int  Logic::GetTgargetCount(Component& c)
{
    ValueComponent* r=(ValueComponent*)c.GetComponent(TARGET);
    return  r->GetValue();
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
    
	ValueComponent* r=(ValueComponent*)e.GetComponent(DEFENSE);
    if(r)
        defense+=r->GetValue();
    
    return defense;
}
bool  Logic::IsDead(Component& c)
{
    return ((ValueComponent*)c.GetComponent(HP))->GetValue()==0;
}
int Logic::DropHp( Component& e,int hp )
{
    assert(hp>=0);
    printf("%d drop blood %d \n",&e,hp);
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

void Logic::CastSkills(Component& caster,Component& suffers,Component& skill )
{
    PackageComponent * pc =(PackageComponent*)&suffers;
    
        int target=GetTgargetCount(skill);
    for (int i=0; i<pc->GetMaxSize() && target>0; ++i, --target) {

            Component* c=pc->GetItem(i);
        
            if(c)
            Attack(skill, *c);
    }
}

//only handle one suffer
void Logic::CastSkill( Component& caster, Component& suffer,Component& skill )
{
	
    if(skill.GetId() == RELIVE)
	{
        if(IsDead(suffer))
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
    }else if(skill.GetId() ==FIREBALL)
    {
        
        
        int ats=GetAttack(skill);
        int def=GetDefense(suffer);

        int hp_diff=ats-def;
        hp_diff=hp_diff<0?0:hp_diff;

        DropHp(suffer,hp_diff);
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
