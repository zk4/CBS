#pragma  once

#include "es/Component.h"
#include "cocos2d.h"
#include "../spine-runtimes/spine-cocos2dx/src/spine/spine-cocos2dx.h"

class SpineComp : public Component , public cocos2d::CCObject {


  public:
    static  SpineComp* Create();
    SpineComp();
    ~SpineComp();

    spine::CCSkeletonAnimation* skeletonNode;
    bool HandleMessage ( const Telegram& msg );
    void animationStateEvent ( spine::CCSkeletonAnimation* node, int trackIndex, spEventType type, spEvent* event, int loopCount );
};