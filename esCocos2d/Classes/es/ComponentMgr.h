#pragma once
#include <vector>
#include <cassert>

using namespace std;

#include "Singleton.h"
class  Component;


#define CompMgr ComponentMgr::GetSingleton()

class ComponentMgr : public Singleton<ComponentMgr> {

  public:
    vector<Component*>  _ComponentMap;
    vector<int>  _aboutToDeleteIds;

    void			Update();
    int				GetAvailableId();
    void            RegisterComponent (Component* NewComponent);
    Component*		GetComponentFromID (int id) const;
    void            RemoveComponent (int id);
};
