#pragma  once

#include <string>
#include "Component.h"
#include <memory>

class Entity:public Component
{

public:
    static  shared_ptr<Entity > Create (string id="");
    Entity (string   id);
    bool  HandleMessage (const Telegram& msg);

};
