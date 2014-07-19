#pragma  once

#include "Component.h"
#include "Configuration.h"
#include "messaging/MessageDispatcher.h"

class WeaponComponent : public Component
{

public:
    WeaponComponent() :Component ("Weapon")
    {
    }
    static shared_ptr<WeaponComponent> Create( )
    {
        return shared_ptr<WeaponComponent> (new WeaponComponent( ));
    }

    bool HandleMessage (const Telegram& msg)
    {
        return false;
    }
    void Attack (int id, double hp)
    {
        Dispatcher->DispatchMessageZZ (0, GetID(), id, HURT, {hp});
    }
};

