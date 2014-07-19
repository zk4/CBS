#pragma  once

#include "Component.h"
#include "Configuration.h"
#include "messaging/MessageDispatcher.h"
#include "cocos2d.h"
using namespace cocos2d;

class  CocosComponent :public Component
{
public:
    CCNode*   _node;
    inline CCNode* Node()
    {
        return _node;
    };
    static shared_ptr<CocosComponent> C ()
    {
        return shared_ptr<CocosComponent> (new CocosComponent ( ));
    }
    ~CocosComponent()
    {
        _node->release();
    }
    CocosComponent ( ) :Component ("Cocos")
    {
        _node=CCSprite::create ( );
        _node->retain();
    }
    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {
        case SET_POS:
        {
            _node->setPosition (ccp ( msg.args[0], msg.args[1] ));
        }
        break;
        default:
            break;
        }
        return false;
    }
};