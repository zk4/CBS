#pragma once 
#include <map>
#include <cassert>
#include <string>
#include <memory>
using namespace std;

#include "Singleton.h"
class  Component;

 
#define CompMgr ComponentMgr::GetSingleton()
 
class ComponentMgr : public Singleton<ComponentMgr> {
private:

	typedef std::map<int, Component*> ComponentMap;

private:
	ComponentMap _ComponentMap;
public:
 
	void            RegisterComponent(Component* NewComponent);
	Component*		GetComponentFromID(int id) const;
	Component*			FindComponentFromID(int id);
	void            RemoveComponent( Component* pComponent);
};
 