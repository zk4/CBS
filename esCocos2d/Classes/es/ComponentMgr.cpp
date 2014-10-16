#include "ComponentMgr.h"
#include "Component.h"


Component* ComponentMgr::GetComponentFromID (int id) const {
    //find the Component
    if (_ComponentMap.size() > id)return _ComponentMap[id];
    return NULL;
}


void ComponentMgr::RemoveComponent (int id) {
    if (_ComponentMap.size() > id)  _ComponentMap[id] = NULL;
}

void ComponentMgr::RegisterComponent (Component* NewComponent) {
    if (NewComponent->GetName() == Entity_) {
        if (_ComponentMap.size() <= NewComponent->GetID())
            _ComponentMap.resize(NewComponent->GetID() + 1);

        _ComponentMap[NewComponent->GetID()] = NewComponent;
    }

}

void ComponentMgr::Update() {

}

int ComponentMgr::GetAvailableId() {
    for (int i = 0; i < _ComponentMap.size(); ++i) {
        if (_ComponentMap[i] == NULL)return i;
    }
    _ComponentMap.resize(_ComponentMap.size() + 1);
    return _ComponentMap.size() - 1;
}

