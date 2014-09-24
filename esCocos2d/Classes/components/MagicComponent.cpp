#include "MagicComponent.h"
#include "es/ComponentMgr.h"
#include "Configuration.h"
#include "messaging/MessageDispatcher.h"
#include "components/Components.h"
#include "particle_nodes/CCParticleExamples.h"
#include "math/zkMath.h"
using namespace cocos2d;
static const char s_fire[] = "fire.png";
MagicComponent::MagicComponent ( CCRenderTexture*  rt ) : Component ( Component_MAGIC ), _rt ( rt ) {
    _emitter = NULL;
    _b_pressed = false;
}
static void  generatePoint ( CCPoint& p, vector<CCPoint>& outer ) {

    outer.push_back ( p );
    auto s = CCDirector::sharedDirector()->getWinSize();
    zkMath::Line l = { 1, 0, -s.width / 2 };
    ;
    outer.push_back ( zkMath::symmetricalPointofLine ( p, l ) );
}
bool MagicComponent::HandleMessage ( const Telegram& msg ) {
    switch ( msg.Msg ) {
    case Telegram_UPDATE: {
        float f = msg.args[0];
        if ( _emitter ) {
            _emitter->update ( f );
            if ( _b_pressed ) {
                _rt->begin();
                auto oldP = _emitter->getPosition();
                static vector<CCPoint>  ps;
                ps.clear();
                generatePoint ( oldP, ps );
                for ( auto & a : ps ) {
                    _emitter->setPosition ( a );
                    _emitter->visit();
                }
                _emitter->setPosition ( oldP );
                _rt->end();
            }
        }
    }
    break;
    case Telegram_TOUCH_BEGIN: {
        CCPoint * world_pos = reinterpret_cast<CCPoint*> ( ( int ) ( msg.args[0] ) );
        _b_pressed = true;
        _emitter = CCParticleGalaxy::create();
        _emitter->retain();
        _emitter->setTexture ( CCTextureCache::sharedTextureCache()->addImage ( s_fire ) );
        _emitter->setPosition ( *world_pos );
// update  anchorpoint
        auto s = CCDirector::sharedDirector()->getWinSize();

    }
    break;
    case	Telegram_TOUCH_MOVE: {
        if ( _b_pressed ) {
            CCPoint * world_pos = reinterpret_cast<CCPoint*> ( ( int ) ( msg.args[0] ) );
            _emitter->setPosition ( *world_pos );
        }
    }
    break;
    case	Telegram_TOUCH_END:
    case	Telegram_TOUCH_CANCEL: {
        _b_pressed = false;
        if ( _emitter ) {
            _emitter->release();
            _emitter = NULL;
        }
    }
    break;
    case Telegram_VISIT: {

        _rt->visit();
    }
    break;

    default:
        break;
    }
    return false;
}
