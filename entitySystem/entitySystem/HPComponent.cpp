
#include "HPComponent.h"

bool HPComponent::HandleMessage (const Telegram& msg)
{
    if (msg.Msg == HURT)
    {
        HP -= (int)msg.args[0];
        if (HP < 0)
        {
            DD (0, GetID(), GetParent()->GetID(), DEAD, {0});
        }
    }

    return false;
}
