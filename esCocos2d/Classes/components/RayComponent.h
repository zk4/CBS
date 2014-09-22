#pragma  once

#include "es/Component.h"
#include "cocos2d.h"
#include "../math/zkMath.h"

class RayComponent : public Component {
    cocos2d::CCPoint				 _ccp_light;
    bool							_b_move_light;
    vector<zkMath::Segment>			_walls;
    vector<cocos2d::CCPoint >		 _edge_nodes;
    cocos2d::CCDrawNode*			 _p_draw_node;

  public:
    static  RayComponent* Create();
    RayComponent();
    ~RayComponent();

    void reConstructScanNodes();
    bool HandleMessage ( const Telegram& msg );
    bool getNearest ( cocos2d::CCPoint& p, cocos2d::CCPoint& out );
    void drawElement ( cocos2d::CCPoint& p, cocos2d::CCPoint  p2 );
};