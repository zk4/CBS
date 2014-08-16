#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "es/Component.h"
using namespace cocos2d;
class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

    // a selector callback
    void menuCloseCallback (CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC (HelloWorld);

    void draw();
    void update (float delta);
    virtual bool ccTouchBegan (CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved (CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled (CCTouch *pTouch, CCEvent *pEvent);
    Component*				man;
    Component  *			bg;
    Component  *			ball;
    Component  *			wall;
    vector< Component* >    bads;
    CCSpriteBatchNode* _batchNode ;
    CCSize				_winsize;
    double					_dTimeNow;
    CCDrawNode*				_drawNode;

};

#endif // __HELLOWORLD_SCENE_H__
