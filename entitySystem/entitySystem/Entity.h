#pragma  once

#include <string>
#include "Component.h"
#include <memory>

class Entity:public Component
{

public:
    static   Entity* Create (eComponent);
    Entity (eComponent);
    bool  HandleMessage (const Telegram& msg);

};
