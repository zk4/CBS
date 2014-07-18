#pragma  once 

#include "Component.h"
#include "Configuration.h"
#include "messaging/MessageDispatcher.h"

class HPComponent : public Component
{
	int HP;
public:
	HPComponent(int hp) :Component("HP"), HP(hp)
	{
	}
	static shared_ptr<HPComponent> Create(int hp)
	{
		return shared_ptr<HPComponent>(new HPComponent(hp));
	}
	bool HandleMessage(const Telegram& msg);
};

