#include "Entity.h"



Entity* Entity::Create (eComponent id )
{
    return  new Entity (id );
}


Entity::Entity (eComponent s) :Component (s) {}

bool Entity::HandleMessage (const Telegram& msg)
{
    for (auto  a : components)
    {
        if (a)
            a ->HandleMessage (msg);

    }
    return true;
}
