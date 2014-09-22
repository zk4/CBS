#pragma  once

#include "es/Component.h"
#include "Configuration.h"
#include "messaging/MessageDispatcher.h"
#include "cocos2d.h"
#include "es/ComponentMgr.h"

#include "fluid/fluidmodel.h"
using namespace cocos2d;

class FluidComponent : public Component {


    bool					_b_move_light;
    CCDrawNode*				_p_draw_node;
    fluid::FluidModel     *fluid;
#define  X 60
#define  Y 60

  public:



    static  FluidComponent* Create() {
        return new FluidComponent();
    }
    ~FluidComponent() {
        delete fluid;
        _p_draw_node->release();
    }


    FluidComponent() : Component ( Component_Fluid ) {
        CCSize winsize = CCDirector::sharedDirector()->getWinSize();

        _p_draw_node = CCDrawNode::create();

        _p_draw_node->retain();
        fluid = new fluid::FluidModel ( winsize.width / 4, winsize.height / 4, X, Y );
    };




    bool HandleMessage ( const Telegram& msg ) {
        switch ( msg.Msg ) {
        case    Telegram_UPDATE: {
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

            CCPoint * world_pos = reinterpret_cast<CCPoint*> ( ( int ) ( msg.args[0] ) );
            CCSize winsize = CCDirector::sharedDirector()->getWinSize();
            fluid->setMovePos ( world_pos->x, winsize.height - world_pos->y );

        }
        break;
        case Telegram_TOUCH_BEGIN: {
            CCSize winsize = CCDirector::sharedDirector()->getWinSize();
            CCPoint * world_pos = reinterpret_cast<CCPoint*> ( ( int ) ( msg.args[0] ) );
            fluid->setMovePos ( world_pos->x, winsize.height - world_pos->y );
            fluid->setPressed ( true );


        }
        break;
        case    Telegram_TOUCH_CANCEL:
        case  Telegram_TOUCH_END: {
            fluid->setPressed ( false );

        }
        break;
        case    Telegram_VISIT: {
            CCSize winsize = CCDirector::sharedDirector()->getWinSize();
            fluid->step();
            const fluid::Line *lines = fluid->getLines();

            _p_draw_node->clear();
            for ( unsigned i = 0; i < X * Y; ++i ) {
                //   _p_draw_node->drawSegment ( ccp ( lines[i].x1 * 4, winsize.height - lines[i].y1 * 4 ), ccp ( lines[i].x2 * 4 + 2, winsize.height - lines[i].y2 * 4 + 2 ), 1, { 0, 1, 0, 1 } );
                _p_draw_node->drawDot ( ccp ( lines[i].x1 * 4, winsize.height - lines[i].y1 * 4 ), 10, { 0,  1, 1, 1 } );

            }
            _p_draw_node->visit();
        }
        break;

        default:
            break;
        }
        return false;

    }


};