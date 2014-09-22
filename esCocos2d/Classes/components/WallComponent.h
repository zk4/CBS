#pragma once
#include "cocos2d.h"
#include "es/Component.h"
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include "algorithm/Path.h"

using namespace cocos2d;

static bool operator== ( const CCPoint& p1, const CCPoint& p2 ) {
    return p1.x == p2.x && p1.y == p2.y;
}

static bool operator!= ( const CCPoint& p1, const CCPoint& p2 ) {
    return  ! ( p1 == p2 );
}
static bool operator< ( const CCPoint& p1, const CCPoint& p2 ) {
    return p1.x < p2.x || ( p1.x == p2.x && p1.y < p2.y );
}


class WallComponent : public Component {
    class SetComp {
      public:
        int operator() ( const CCPoint& p1, const CCPoint& p2 ) {
            return p1.x < p2.x || ( p1.x == p2.x && p1.y < p2.y );
        }
    };

    std::mutex					_cv_m;
    std::condition_variable		_cv;
    algorithm::Graph<CCPoint>	_graph;
    CCPoint						_ccp_RL;
    vector < CCPoint>		 _nodes;
    list<CCPoint>				_shorest;
    int							_width;
    std::thread					_thread;
    CCLabelTTF*					_access_count;
    CCPoint 					_start;
    CCPoint 					_end;
    bool						_bChanged;
#define  WallComponents_none  -1
#define  WallComponents_start -2
#define  WallComponents_end   -3
    int							_last_ite;
    struct ccp_ext {
        CCPoint  p;
        int      iAccessTimes;
        CCPoint  direction;
    };
    vector<ccp_ext>			    _access_nodes;
  public:
    static  WallComponent* Create ( int width );
    WallComponent ( int w );
    ~WallComponent();

    void setWall ( CCPoint p );
    void MakeGraph ( CCPoint  right_left );
    void construct();
    bool HandleMessage ( const Telegram& msg );

};