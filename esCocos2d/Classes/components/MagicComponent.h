#pragma  once

#include "es/Component.h"
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;

class MagicComponent : public Component
{

public:
    CCRenderTexture*  _rt;
    CCParticleSystem*    _emitter;
    bool  _b_pressed;
    MagicComponent (CCRenderTexture*  rt);
    ~MagicComponent()
    {

    }
    static  MagicComponent* Create (CCRenderTexture*  rt)
    {
        return new MagicComponent (rt);
    }

    bool HandleMessage (const Telegram& msg);


};