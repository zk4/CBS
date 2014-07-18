 
#include "HPComponent.h"

bool HPComponent::HandleMessage(const Telegram& msg)
{
	if (msg.Msg == HURT)
	{
		HP -= (int)msg._args[0];
		if (HP < 0)
		{
			Dispatcher->DispatchMessageZZ(0, GetID(), GetParent()->GetID(), DEAD, {(void*)0});
		}
	}
	return false;
}
