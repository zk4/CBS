#pragma  once

#include "es/Component.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Perlin_Noise/ppm.h"

using namespace cocos2d;

class MagicComponent : public Component {

  public:
    CCRenderTexture*  _rt;
    CCSprite*    _pBrush;
    CCParticleSystem*    _emitter;
    ppm image;
    bool  _b_pressed;
    CCDrawNode* _drawNode;
    MagicComponent ( CCRenderTexture*  rt );
    ~MagicComponent();
    static  MagicComponent* Create ( CCRenderTexture*  rt ) {
        return new MagicComponent ( rt );
    }

    bool HandleMessage ( const Telegram& msg );


};