#include "ComponentMgr.h"
#include "Component.h"


Component* ComponentMgr::GetComponentFromID(int id) const
{
    //find the Component
    ComponentMap::const_iterator ent = _ComponentMap.find ( id );
    assert ( ( ent !=  _ComponentMap.end() ) && "<ComponentMgr::GetComponentFromID>: invalid ID" );
    return ent->second;
}


void ComponentMgr::RemoveComponent(Component* pComponent)
{
	auto find_ = _ComponentMap.find(pComponent->GetID());
	if (find_!=_ComponentMap.end())
	_ComponentMap.erase(find_);
}

void ComponentMgr::RegisterComponent(Component* NewComponent)
{
	_ComponentMap.insert(std::make_pair(NewComponent->GetID(), NewComponent));
}

Component* ComponentMgr::FindComponentFromID(int id)
{
    //find the Component
    ComponentMap::const_iterator ent = _ComponentMap.find ( id );
    
    if ( ent == _ComponentMap.end() )
    { return NULL; }
    
    else
    { return ent->second; }
}
