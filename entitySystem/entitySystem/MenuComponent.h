#pragma  once

#include "Component.h"
#include "Configuration.h"
#include "messaging/MessageDispatcher.h"
#include "cocos2d.h"
using namespace cocos2d;

class  MenuComponent :public Component
{
public:
    CCMenu*   _memu;
    CCMenuItemImage* _item;
    static shared_ptr<MenuComponent> C ( CCMenuItemImage*  item )
    {
        return shared_ptr<MenuComponent> (new MenuComponent (item ));
    }
    ~MenuComponent()
    {
        _memu->release();
    }
    MenuComponent ( CCMenuItemImage*  item) :Component ("Menu")
    {
        _memu = CCMenu::create (item,NULL);
        _item=item;
        _memu->retain();
    }
    bool HandleMessage (const Telegram& telegram)
    {

        return false;
    }
};