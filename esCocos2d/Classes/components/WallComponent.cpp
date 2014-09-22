#include "WallComponent.h"




void WallComponent::MakeGraph ( CCPoint right_left ) {
    _graph.clear();


    for ( int x = 0; x <= right_left.x; ++x ) {

        for ( int y = 0; y <= right_left.y; ++y ) {

            CCPoint p = ccp ( x, y );
            CCPoint p4[] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } /* , { 1,1 }, { -1, 1 }, { -1, -1 }, { 1, -1 }   */ };
            for ( auto& go : p4 ) {

                CCPoint possibale = p + go;


                _graph.addNEdge ( ( p ), ( possibale ), go.getLength() );

            }
        }

    }
}

WallComponent::WallComponent ( int w ) : Component ( Component_WALL ), _width ( w ), _thread ( std::bind ( &WallComponent::construct, this ) ) {
    _ccp_RL = ccp ( 50, 50 );
    MakeGraph ( _ccp_RL );
    _access_count = CCLabelTTF::create ( "", "Helvetica", 16 );
    _access_count->retain();
    _start = ccp ( 5, 5 );
    _end = CCPointZero;
    _last_ite = -1;
    _bChanged = false;
}

void WallComponent::construct() {
    while ( true ) {
        std::unique_lock<std::mutex> lk ( _cv_m );
        CCPoint oldend = _end;
        CCPoint oldstart = _start;
        _cv.wait ( lk, [ = ] {return  oldend != _end || _start != oldstart || _bChanged == true; } );

        _bChanged = false;

        _shorest.clear();

        auto from = _graph.findNode ( _start );
        auto  current = _graph.findNode ( _end );


        {
            _graph.AStar ( from, current, [] ( const CCPoint & a, const CCPoint & b ) {
                return  a.getDistance ( b ) * 1.2;
            } );
        }



        while ( current && current != from ) {
            _shorest.push_back ( current->data );
            current = current->p;
        }

        _cv.notify_all();
    }
}

void WallComponent::setWall ( CCPoint p ) {
    _graph.SetNodeValidate ( p, false );
    auto ite = find_if ( _nodes.begin(), _nodes.end(), [&p] ( CCPoint & pp ) {
        return p == pp;
    } );
    if ( ite == _nodes.end() )
        _nodes.push_back ( p );
}

WallComponent* WallComponent::Create ( int width ) {
    return new WallComponent ( width );
}

WallComponent::~WallComponent() {

}

bool WallComponent::HandleMessage ( const Telegram& msg ) {
    switch ( msg.Msg ) {

    case    Telegram_UPDATE: {

    }
    break;
    case Telegram_ACCESS_NODE: {

        CCPoint * world_pos = reinterpret_cast<CCPoint*> ( ( int ) ( msg.args[0] ) );
        //  CCPoint * world_parent_pos = reinterpret_cast<CCPoint*> ((int) (msg.args[1]));
        CCPoint  direction = *world_pos;// ccpSub(*world_parent_pos, *world_pos).normalize();
        auto  ite = find_if ( _access_nodes.begin(), _access_nodes.end(), [ = ] ( const  ccp_ext & ce ) {
            return ce.p == *world_pos;
        } );
        if ( ite != _access_nodes.end() ) {
            ite->iAccessTimes++;
            ite->direction = direction;
        } else {
            _access_nodes.push_back ( { *world_pos, 1 } );
            _access_nodes.rbegin()->direction = direction;
        }
    }
    break;
    case  Telegram_TOUCH_DOUBLE: {
        CCPoint * world_pos = reinterpret_cast<CCPoint*> ( ( int ) ( msg.args[0] ) );

        for ( auto a = _nodes.begin(); a != _nodes.end(); ++a ) {
            CCRect r = CCRectMake ( a->x * _width, a->y * _width, _width, _width );
            if ( r.containsPoint ( *world_pos ) ) {
                _graph.SetNodeValidate ( *a, true );
                _nodes.erase ( a );
                break;
            }
        }
        _bChanged = true;
        _cv.notify_all();
    }
    break;
    case Telegram_TOUCH_MOVE: {
        CCPoint * world_pos = reinterpret_cast<CCPoint*> ( ( int ) ( msg.args[0] ) );
        int x = world_pos->x / _width;
        int y = world_pos->y / _width;
        if ( _last_ite == WallComponents_start ) {
            _start = ccp ( x, y );
        } else if ( _last_ite == WallComponents_end ) {
            _end = ccp ( x, y );
        } else if ( _last_ite != WallComponents_none ) {
            if ( _nodes.size() > _last_ite ) {
                _graph.SetNodeValidate ( _nodes[_last_ite], true );
                _nodes[_last_ite] = ccp ( x, y );
                setWall ( ccp ( x, y ) );
            }

        } else {
            setWall ( ccp ( x, y ) );
        }
        _bChanged = true;
        _cv.notify_all();

    }
    break;
    case Telegram_TOUCH_BEGIN: {

        CCPoint * world_pos = reinterpret_cast<CCPoint*> ( ( int ) ( msg.args[0] ) );

        int x = world_pos->x / _width;
        int y = world_pos->y / _width;

        _last_ite = WallComponents_none;
        for ( int i = 0; i < _nodes.size(); ++i ) {
            if ( ccp ( x, y ) == _nodes[i] ) {
                _last_ite = i;
                break;
            }
        }
        {
            CCRect r = CCRectMake ( _start.x * _width, _start.y * _width, _width, _width );
            if ( r.containsPoint ( *world_pos ) ) {
                _last_ite = WallComponents_start;
            }
        }

        {
            CCRect r = CCRectMake ( _end.x * _width, _end.y * _width, _width, _width );
            if ( r.containsPoint ( *world_pos ) ) {
                _last_ite = WallComponents_end;
            }
        }


        if ( _last_ite == WallComponents_none )
            setWall ( ccp ( x, y ) );



        _bChanged = true;
        _cv.notify_all();



    }
    break;
    case    Telegram_TOUCH_CANCEL:
    case  Telegram_TOUCH_END: {
        _last_ite = WallComponents_none;
    }
    break;
    case    Telegram_VISIT: {
        {

            int i = 0;

            for ( auto a : _shorest ) {
                if ( i == 0 ) {
                    ++i;
                    continue;;
                }
                if ( i == _shorest.size() ) break;

                ccColor4F c = { 1, 1, 1, 1 };

                ccDrawSolidRect ( ccp ( a.x * _width, a.y * _width ), ccp ( ( a.x + 1 ) *_width, ( a.y + 1 ) *_width ), c );
                ++i;
                if ( i >= _shorest.size() ) break;
            }

        }

        {
            static float middle = 0.3f;
            for ( auto & p : _nodes )
                ccDrawSolidRect ( p * _width, ccpAdd ( p, { 1, 1 } ) *_width, { 1, middle, middle, 1

                                                                              } );
            ccDrawSolidRect ( ccp ( _start.x * _width, _start.y * _width ), ccp ( ( _start.x + 1 ) *_width, ( _start.y + 1 ) *_width ), { 1, 1, middle, 1 } );
            ccDrawSolidRect ( ccp ( _end.x * _width, _end.y * _width ), ccp ( ( _end.x + 1 ) *_width, ( _end.y + 1 ) *_width ), { middle, middle, 1, 1 } );




        }
        //{
        //    //draw bg
        //    for (auto a : _graph._OL->nodes)
        //    {
        //        auto edge = a->get_nextOut();
        //        while (edge)
        //        {

        //            CCPoint from = (edge->fromNode->data);
        //            CCPoint to = (edge->toNode->data);
        //            ccDrawColor4F (255, 255, 255, 255);
        //            ccDrawLine (ccp (from.x*_width, from.y*_width), ccp (_width*to.x, _width*to.y));

        //            edge = edge->nextOutedge;
        //        }
        //    }
        //}

        //            {
        ////draw access
        //
        //                int size = _access_nodes.size();
        //                for (int i =0; i < size; ++i)
        //                {
        //
        //                    ccp_ext p = _access_nodes[i];
        //                    if (p.direction.x<0)p.direction.x = -1;
        //                    if (p.direction.x>0)p.direction.x =  1;
        //                    if (p.direction.y<0)p.direction.y = -1;
        //                    if (p.direction.y>0)p.direction.y = 1;
        //                    float ff = p.iAccessTimes / 4000.0f + 0.5f;
        ////                    ccDrawSolidRect(ccp(p.p.x*_width, p.p.y*_width), ccp((p.p.x + 1)*_width, (p.p.y + 1)*_width), { ff, ff, ff, ff});
        //                    // static char buffer[4];
        //                    // sprintf (buffer, "%i ", p.iAccessTimes);
        //                    //  _access_count->setString (buffer);
        //                    _access_count->setPosition (ccp (p.p.x*_width+_width/2, p.p.y*_width+_width/2));
        //                    // _access_count->visit();
        //                    static struct
        //                    {
        //                        CCPoint dir;
        //                        const char* symb;
        //                    } directions[]=
        //                    {
        //                        { { 1, 0  }, "<" },
        //                        { { -1, 0 }, ">" },
        //                        { { 0, 1  }, "^" },
        //                        { { 0, -1 }, "v" },
        //                        { { 0, 0  }, "X" },
        //                        { { 1, 1 },  "^>" },
        //                        { { -1, 1 }, "<^" },
        //                        { { -1, -1 }, "<v" },
        //                        { { 1, -1 }	 ,"v>" },
        //
        //                    };
        //                    for (auto & a : directions)
        //                    {
        //                        if (a.dir == p.direction)
        //                        {
        //                            _access_count->setString (a.symb);
        //                            // _access_count->setString ("X");
        //                            _access_count->visit();
        //                        }
        //
        //
        //
        //                    }
        //                }
        //
        //            }
        //
    }
    break;

    default:
        break;
    }
    return false;
}
