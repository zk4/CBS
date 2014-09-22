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
    skeletonNode = CCSkeletonAnimation::createWithFile ( "spineboy.json", "spineboy.atlas" );
    skeletonNode->setMix ( "walk", "jump", 0.2f );
    skeletonNode->setMix ( "jump", "walk", 0.4f );

    skeletonNode->setAnimationListener ( this, animationStateEvent_selector ( SpineComp::animationStateEvent ) );
    skeletonNode->setAnimation ( 0, "walk", false );
    skeletonNode->addAnimation ( 0, "jump", false );
    skeletonNode->addAnimation ( 0, "walk", true );
    skeletonNode->addAnimation ( 0, "jump", true, 3 );

    skeletonNode->debugBones = true;
    skeletonNode->update ( 0 );

    skeletonNode->runAction ( CCRepeatForever::create ( CCSequence::create ( CCFadeOut::create ( 1 ),
                              CCFadeIn::create ( 1 ),
                              CCDelayTime::create ( 5 ),
                              NULL ) ) );


    skeletonNode->retain();
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
        skeletonNode->update ( delta );
    }
    break;
    case Telegram_SET_POS: {
        CCPoint start = ccp ( msg.args[0], msg.args[1] );

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

    }
    break;
    case    Telegram_TOUCH_CANCEL:
    case  Telegram_TOUCH_END: {

    }
    break;
    case    Telegram_DRAW: {
        skeletonNode->draw();
    }
    break;

    default:
        break;
    }
    return false;
}
