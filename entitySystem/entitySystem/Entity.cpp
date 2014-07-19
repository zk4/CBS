#include "Entity.h"



shared_ptr<Entity > Entity::Create (string id )
{
    return shared_ptr<Entity> (new Entity (id));
}


Entity::Entity (string id) :Component (id) {}

bool Entity::HandleMessage (const Telegram& msg)
{
    for (auto  a : components)
    {
        a->HandleMessage (msg);

    }
    return true;
}
