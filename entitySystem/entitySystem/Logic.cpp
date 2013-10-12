#include "Logic.h"
#include "Component.h"
#include <algorithm>

#include "ValueComponent.h"
#include "PackageComponent.h"

typedef ValueComponent<int> cint;
typedef ValueComponent<float> cfloat;
Logic::Logic(void)
{
}


Logic::~Logic(void)
{
}
void Logic::CastSkill(Component& attack, Component& defender,string id )
{
  	PackageComponent*  p=(PackageComponent*)attack.GetC(SKILLARRAY);
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
    PackageComponent*  p=(PackageComponent*)caster.GetC(SKILLARRAY);
    if(p)
    {
        Component* c =p->GetItem(id);
        if(c)
        {
            
            CastSkills(caster,suffers,*c);
        }
    }
}
//void Logic::Attack( Component& attack, Component& defender )
//{
//	int atk=GetAttack(attack);
//	int def=GetDefense(defender);
//	int hp_diff=atk-def;
//    if(hp_diff<0)hp_diff=0;
//    printf("drop hp:%d\n",hp_diff);
//    DropHp(defender,hp_diff);
//}

int Logic::GetAttack( Component& e )
{
	 
	 
	PackageComponent* hands=(PackageComponent*)e.GetC(HANDS);
		int atk=0;
    if(hands)
	{
		for_each(hands->GetBeginIte(), hands->GetEndIte(), [&atk](Component* c){
			 
			if(c)
			{
                vector<Component*> atks;
				c->GetCs(ATTACK,atks,true);
				for (auto a2: atks) {
                    atk+=((cint*)a2)->Get();
                }
					
			}
		}
		);
    }
		cint* r=(cint*)e.GetC(ATTACK);
		if(r)
			atk+=r->Get();

		return atk;
	 
}
int  Logic::GetTgargetCount(Component& c)
{
    cint* r=(cint*)c.GetC(TARGET);
    return  r->Get();
}
int  Logic::GetDefense(Component& e )
{
 
PackageComponent* hands=(PackageComponent*)e.GetC(HANDS);

	int defense=0;
    if(hands)
	{
        
        for_each(hands->GetBeginIte(), hands->GetEndIte(), [&defense](Component* c){
           
            if(c)
            {
                vector<Component*> defenses;
                 c->GetCs(DEFENSE,defenses,true);
                for (auto a2: defenses) {
                    defense+=((cint*)a2)->Get();
                }
                
            }
        }
        );
     }
    
	cint* r=(cint*)e.GetC(DEFENSE);
    if(r)
        defense+=r->Get();
    
    return defense;
}
bool  Logic::IsDead(Component& c)
{
    return ((cint*)c.GetC(HP)->GetC(NOWINT))->Get()==0;
}
int  Logic::AddHp(Component& e,int hp)
{
    cint* nowint=(cint*)(e.GetC(HP)->GetC(NOWINT));
    if(nowint)
    nowint->Add(hp);
    cint* maxint=(cint*)(e.GetC(HP)->GetC(MAXINT));
    if(maxint)
    {
        int maxv=maxint->Get();
        if(nowint->Get()>maxv)
            nowint->Set(maxv);
    }
    
}
int Logic::DropHp( Component& e,int hp )
{
    assert(hp>=0);
    printf("%d drop blood %d \n",&e,hp);
	return ((cint*)(e.GetC(HP)->GetC(NOWINT)))->Minus(hp);
}

Component* Logic::Equip( Component& holder,Component& equip,int which_hand )
{
	PackageComponent*  ics=(PackageComponent*)holder.GetC(HANDS);
    return 	ics->SetItem(&equip,which_hand);
}

int Logic::GetHp( Component& e )
{
	return ((cint*)e.GetC(HP)->GetC(NOWINT))->Get();
}

void Logic::CastSkills(Component& caster,Component& suffers,Component& skill )
{
    PackageComponent * pc =(PackageComponent*)&suffers;
    
        int target=GetTgargetCount(skill);
    for (int i=0; i<pc->GetMaxSize() && target>0; ++i, --target) {

            Component* c=pc->GetItem(i);
        
            if(c)
        CastSkill(caster,*c,skill);
    }
}

void Logic::AddSkillStatus(Component& suffer,Component& skillstatus)
{
    PackageComponent* skillstatus_added=dynamic_cast<PackageComponent*>(suffer.GetC(SKILLSTATUS_ADDED)) ;
    if(skillstatus_added)
    {
        skillstatus_added->AddItem(&skillstatus);
    }
}

//only handle one suffer
void Logic::CastSkill( Component& caster, Component& suffer,Component& skill )
{
	
    if(skill.GetId() == RELIVE)
	{
        if(IsDead(suffer))
            {
                    PackageComponent*  p=(PackageComponent*)suffer.GetC(SKILLARRAY);
                    if(p)
                    {
                        Component* c =p->GetItem(RELIVE);
                            if(c)
                            {
                                    printf("relive\n");
                                  int MAX=  ((cint*)suffer.GetC(HP)->GetC(MAXINT))->Get();
                                
                            }
                    }
            
            }
    }else if(skill.GetId() ==FIREBALL)
    {
        
        
        int ats=GetAttack(skill);
        for(Component* boost= caster.GetC(BOOST);boost!=NULL;)
        {
            for (cfloat* fireboost=(cfloat*)boost->GetC(FIRE_BOOST); fireboost!=NULL; ) {
                ats*=(1+fireboost->Get());
                break;
            }
            
            break;
        }
        for(Component* c= suffer.GetC(RESISTANCE);c!=NULL;)
        {
            for (cfloat* fireboost=(cfloat*)c->GetC(FIRE_RESISTANCE); fireboost!=NULL; ) {
                ats*=(1-fireboost->Get());
                break;
            }
            
            break;
        }
        
        //attack
        int def=GetDefense(suffer);
        int hp_diff=ats*((float)ats/(ats+def));
        hp_diff=hp_diff<0?0:hp_diff;
        DropHp(suffer,hp_diff);
        
        //skillstatus
        for(Component* skillstatus= skill.GetC(SKILLSTATUS);skillstatus!=NULL;)
        {
            if(skillstatus)
            {
                Component* c=new Component(SKILLSTATUS);
              Component* ss=skillstatus->Copy(c);
            AddSkillStatus(suffer, *ss);
            }
            
            break;
        }
        
        
    }
    
    
    
}

void Logic::EquipSKill( Component& holder,Component& skill )
{
	PackageComponent* p=(PackageComponent*)holder.GetC(SKILLARRAY);
	if(p)
	{
		p->AddItem(&skill);
	}
	
}


bool Logic::TeamAdd(Component& team,Component& item)
{
    PackageComponent* p=(PackageComponent*)(team.GetC(SKILLARRAY));
    return p->AddItem(&item);
}
