#include "SpineComp.h"
#include "es/ComponentMgr.h"
#include "Configuration.h"
#include "messaging/MessageDispatcher.h"
using namespace cocos2d;
using namespace spine;
SpineComp* SpineComp::Create() {
    return new SpineComp();
}

SpineComp::~SpineComp() {

}

SpineComp::SpineComp() : Component ( Component_Spine ) {
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    _skeletonNode = CCSkeletonAnimation::createWithFile ( "unit_axe_orc.json", "unit_axe_orc.atlas" );
    _skeletonNode->setMix ( "stand", "attack", 0.2f );
    _skeletonNode->setMix ( "attack", "stand", 0.4f );
    _skeletonNode->setSkin ( "2" );
    _skeletonNode->setSlotsToSetupPose();
    _skeletonNode->setAnimationListener ( this, animationStateEvent_selector ( SpineComp::animationStateEvent ) );
    _skeletonNode->setAnimation ( 0, "stand", false );
    _skeletonNode->addAnimation ( 0, "attack", false );
    _skeletonNode->addAnimation ( 0, "attack", true );
    _skeletonNode->addAnimation ( 0, "stand", true, 3 );
//    _skeletonNode->addAnimation ( 1, "drawOrder", true );
    _skeletonNode->debugBones = true;
    _skeletonNode->update ( 0 );
    _skeletonNode->onEnter();
    _skeletonNode->runAction ( CCRepeatForever::create ( CCSequence::create ( CCFadeOut::create ( 1 ),
                               CCFadeIn::create ( 1 ),
                               CCDelayTime::create ( 5 ),
                               NULL ) ) );

//   CCDirector::sharedDirector()->getRunningScene()->addChild ( _skeletonNode );
    _skeletonNode->retain();
}
void SpineComp::animationStateEvent ( CCSkeletonAnimation* node, int trackIndex, spEventType type, spEvent* event, int loopCount ) {
    spTrackEntry* entry = spAnimationState_getCurrent ( node->state, trackIndex );
    const char* animationName = ( entry && entry->animation ) ? entry->animation->name : 0;

    switch ( type ) {
    case ANIMATION_START:
        CCLog ( "%d start: %s", trackIndex, animationName );
        break;
    case ANIMATION_END:
        CCLog ( "%d end: %s", trackIndex, animationName );
        break;
    case ANIMATION_COMPLETE:
        CCLog ( "%d complete: %s, %d", trackIndex, animationName, loopCount );
        break;
    case ANIMATION_EVENT:
        CCLog ( "%d event: %s, %s: %d, %f, %s", trackIndex, animationName, event->data->name, event->intValue, event->floatValue, event->stringValue );
        break;
    }
    fflush ( stdout );
}

bool SpineComp::HandleMessage ( const Telegram& msg ) {
    switch ( msg.Msg ) {
    case    Telegram_UPDATE: {
        float delta = msg.args[0];
        // _skeletonNode->update ( delta );
    }
    break;
    case Telegram_SET_POS: {

    }
    break;
    case Telegram_ACCESS_NODE: {
    }
    break;
    case  Telegram_TOUCH_DOUBLE: {

    }
    break;
    case Telegram_TOUCH_MOVE: {

    }
    break;
    case Telegram_TOUCH_BEGIN: {
        CCPoint * world_pos = reinterpret_cast<CCPoint*> ( ( int ) ( msg.args[0] ) );
        _skeletonNode->setPosition ( *world_pos );

    }
    break;
    case    Telegram_TOUCH_CANCEL:
    case  Telegram_TOUCH_END: {

    }
    break;
    case    Telegram_VISIT: {
        _skeletonNode->visit();
    }
    break;

    default:
        break;
    }
    return false;
}
