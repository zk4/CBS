#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "HexCoordinate.h"
#include  <vector>
using namespace std;
class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    HelloWorld();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();


    void onEnter ();
    // implement the "static node()" method manually
    CREATE_FUNC (HelloWorld);
    virtual bool ccTouchBegan (CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved (CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled (CCTouch *pTouch, CCEvent *pEvent);
    void draw();
    HexCoordinate   field;
    HexCoordinate   line;
    HexCoordinate   intersection;
    //  HexCoordinate  touch_field;
};

#endif // __HELLOWORLD_SCENE_H__
