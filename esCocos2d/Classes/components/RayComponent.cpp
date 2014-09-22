#include "RayComponent.h"
#include "es/ComponentMgr.h"
#include "Configuration.h"
#include "messaging/MessageDispatcher.h"
using namespace cocos2d;
RayComponent* RayComponent::Create() {
    return new RayComponent();
}

RayComponent::~RayComponent() {
    _p_draw_node->release();
}

void RayComponent::reConstructScanNodes() {
    _edge_nodes.clear();
    for ( auto &a : _walls ) {
        _edge_nodes.push_back ( a.s );
        _edge_nodes.push_back ( a.e );
        //add intersection
        for ( auto &b : _walls ) {
            CCPoint  i;
            if ( zkMath::intersects ( a.s, a.e, b.s, b.e, i ) ) {

                _edge_nodes.push_back ( i );
            }

        }
    }
}

RayComponent::RayComponent() : Component ( Component_Raytrace ) {
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    _walls = {
        {
            { 100, 100 }, { 700, 100 }
        },
        {
            { 400, 50 }, { 300, 50 }
        },
        {
            { 300, 200 }, { 500, 200 }
        },
        {
            { 50, 50 }, { 50, s.height - 50 }
        },
        {
            { 50, 50 }, { s.width - 50, 50 }
        },
        {
            { 50, s.height - 50 }, { s.width - 50, s.height - 50 }
        },
        {
            { s.width - 50, 50 }, { s.width - 50, s.height - 50 }
        },
        {
            { 700, 200 }, { 300, 500 }
        }
        ,
        {
            { 100, 200 }, { 600, 500 }
        },


        //cube
        {
            { 200, 200 }, { 200, s.height - 200 }
        },
        {
            { 200, 200 }, { s.width - 200, 200 }
        },
        {
            { 200, s.height - 200 }, { s.width - 200, s.height - 200 }
        },
        {
            { s.width - 200, 200 }, { s.width - 200, s.height - 200 }
        },

        //world
        {
            { -300, -300 }, { -300, s.height - -300 }
        },
        {
            { -300, -300 }, { s.width - -300, -300 }
        },
        {
            { -300, s.height - -300 }, { s.width - -300, s.height - -300 }
        },
        {
            { s.width - -300, -300 }, { s.width - -300, s.height - -300 }
        },
    };
    _ccp_light = ccp ( 400, 300 );

    _b_move_light = false;


    _p_draw_node = CCDrawNode::create();
    _p_draw_node->retain();


    reConstructScanNodes();
}

bool RayComponent::HandleMessage ( const Telegram& msg ) {
    switch ( msg.Msg ) {
    case    Telegram_UPDATE: {
    }
    break;
    case Telegram_SET_POS: {
        CCPoint start = ccp ( msg.args[0], msg.args[1] );
        _ccp_light = start;
    }
    break;
    case Telegram_ACCESS_NODE: {
    }
    break;
    case  Telegram_TOUCH_DOUBLE: {
    }
    break;
    case Telegram_TOUCH_MOVE: {
        if ( _b_move_light ) {
            CCPoint * world_pos = reinterpret_cast<CCPoint*> ( ( int ) ( msg.args[0] ) );
            _ccp_light = *world_pos;
        }

    }
    break;
    case Telegram_TOUCH_BEGIN: {
        CCPoint * world_pos = reinterpret_cast<CCPoint*> ( ( int ) ( msg.args[0] ) );
        _ccp_light = *world_pos;
        _b_move_light = true;

    }
    break;
    case    Telegram_TOUCH_CANCEL:
    case  Telegram_TOUCH_END: {
        _b_move_light = false;
    }
    break;
    case    Telegram_VISIT: {
        ccDrawColor4B ( 255, 0, 0, 255 );
        for ( auto& a : _walls ) {
            ccDrawLine ( a.s, a.e );
        }
        ccPointSize ( 10 );
        ccDrawPoint ( _ccp_light );

        ccDrawColor4B ( 255, 255, 255, 255 );
        static vector<CCPoint>   lightEnds;
        lightEnds.clear();
        for ( auto& n : _edge_nodes ) {
            CCPoint p;


            if ( getNearest ( n, p ) )
                lightEnds.push_back ( p );
            CCPoint pp1 = zkMath::rotate ( _ccp_light, 0.001, n );
            if ( getNearest ( pp1, p ) )
                lightEnds.push_back ( p );
            CCPoint pp2 = zkMath::rotate ( _ccp_light, -0.001, n );
            if ( getNearest ( pp2, p ) )
                lightEnds.push_back ( p );




        }
        _p_draw_node->clear();
        sort ( lightEnds.begin(), lightEnds.end(), [ = ] ( CCPoint & c, CCPoint & b ) {

            return ( c - _ccp_light ).getAngle() < ( b - _ccp_light ).getAngle();
        } );

        for ( int i = 0; i < lightEnds.size() - 1; ++i ) {
            CCPoint triangle[3] = { _ccp_light, lightEnds[i], lightEnds[i + 1] };
            _p_draw_node->drawPolygon ( triangle, 3, { 1, 1, 1, 0.5 }, 1, { 1, 1, 1, 0 } );

        }
        CCPoint triangle[3] = { *lightEnds.rbegin(), _ccp_light, lightEnds[0] };
        _p_draw_node->drawPolygon ( triangle, 3, { 1, 1, 1, 0.5 }, 1, { 1, 1, 1, 0 } );
        int i = 0;
        /*  for (auto p : lightWallSegments)
         {

         _p_ttf_index->setPosition (  p );
         char buf[10];
         sprintf (buf, "%f", ccpAngle (_ccp_light, p));
         _p_ttf_index->setString ( buf );

         _p_ttf_index->visit();
         ++i;
         }*/


        _p_draw_node->visit();
    }
    break;

    default:
        break;
    }
    return false;
}

bool RayComponent::getNearest ( CCPoint& p, CCPoint& out ) {
    zkMath::Ray beam = { _ccp_light, ( p - _ccp_light ).normalize() * 99999 };
    CCPoint nearest = beam.d;
    bool in = false;
    for ( auto & wall : _walls ) {

        kmRay2 r = { beam.s.x, beam.s.y, beam.d.x, beam.d.y };
        kmVec2 s = { wall.s.x, wall.s.y };
        kmVec2 e = { wall.e.x, wall.e.y };
        kmVec2 v;

        if ( kmRay2IntersectLineSegment ( &r, &s, &e, &v ) ) {
            CCPoint intersection = { v.x, v.y };

            //已存点距离大于当前找到点
            if ( ( nearest - _ccp_light ).getLength() > ( intersection - _ccp_light ).getLength() )
                nearest = intersection;
            in = true;
        }
    }
    out = nearest;
    return in;
}

void RayComponent::drawElement ( CCPoint& p, CCPoint p2 ) {
    _p_draw_node->drawSegment ( p, p2, 1, ccc4f ( 1, 1, 1, 1 ) );
    _p_draw_node->drawDot ( p, 15, ccc4f ( 1, 1, 1, 1 ) );
    _p_draw_node->drawDot ( p2, 15, ccc4f ( 1, 1, 1, 1 ) );
}
