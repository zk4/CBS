#include "HelloWorldScene.h"
#include "RayComponent2.h"

#include "Components.h"
#include "FluidComponent.h"
USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild ( layer );

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    _winsize = CCDirector::sharedDirector()->getWinSize();
    _drawNode=CCDrawNode::create();
    _drawNode->retain();
    // Create batch node
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile ( "Sprites.plist" );
//    _batchNode = CCSpriteBatchNode::create ( "Sprites.pvr.ccz" );
//    addChild ( _batchNode );

    // bg = Component::Create (Entity_);
    // bg->AddC(SpriteComponent::Create(CCSprite::create("worldmap.png")));
    // bg->AddC (MoveComponent::Create ({ 100, 100 }, 1, 300, 300, 100,1));
    // DD(bg->GetID(), Telegram_SET_POS, { visibleSize.width / 2, visibleSize.height / 2 });


    wall = Component::Create ( Entity_ );
    wall->AddC ( FluidComponent::Create() );
    //  wall->AddC (RadarComponent::Create (50));
    // wall->AddC (MoveComponent::Create ({ 100, 100 }, 1, 100, 1000, 100, 1));
    //  DD (wall->GetID(), Telegram_SET_POS, { 0,0 });

    // DD (wall->GetID(), Telegram_SET_POS, { double (200 + rand() % 30), double (200 + rand() % 30) });
    _dTimeNow = 0;
    srand ( time ( NULL ) );

    auto _rt = CCRenderTexture::create ( _winsize.width, _winsize.height, kCCTexture2DPixelFormat_RGBA8888 );
    _rt->setPosition ( _winsize / 2 );
    _rt->retain();

    for ( int i = 0; i <  5; ++i )
    {

        auto   planes = Component::Create ( Entity_ );
        auto sprite = CCSprite::create ( "airplane.png" );
        sprite->setScale ( .05f );
        CCMenuItemImage* cc = CCMenuItemImage::create ( "CloseNormal.png", "CloseSelected.png", "CloseSelected.png", this, menu_selector ( HelloWorld::menuCloseCallback ) );
        CCMenu* c = CCMenu::create ( cc, NULL );
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate ( c, 0, false );
        sprite->addChild ( c );
        sprite->setPosition ( CCPointZero );
        cc->setPosition ( CCPointZero );
        c->setPosition ( CCPointZero );
        planes->AddC (TrailComponent::Create (_rt));
        planes->AddC ( RadarComponent::Create ( 150 ) );
        planes->AddC ( MoveComponent::Create ( { 100, 100 }, 1, 100, 1000, 100, 0.01  ) );
        //  planes->AddC ( SpriteComponent::Create ( sprite ) );

        //planes->AddC (HPComponent::Create (100));
        // planes->AddC (WeaponComponent::Create());
        bads.push_back ( planes );

        DD ( planes->GetID(), Telegram_SET_POS, { double ( 200 + rand() % 30 ), double ( 200 + rand() % 30 ) } );
    }


    setTouchEnabled ( true );
    setTouchMode ( kCCTouchesOneByOne );
    scheduleUpdate();
    return true;
}


void HelloWorld::menuCloseCallback ( CCObject* pSender )
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit ( 0 );
#endif
}


bool HelloWorld::ccTouchBegan ( CCTouch *pTouch, CCEvent *pEvent )
{

    auto  target = pTouch->getLocation();

    //  auto a = bads[0];
    /*  {

          MoveComponent* m = dynamic_cast<MoveComponent*> (a->GetC (Component_MOVE));

          m->_pos = target;
      }*/
    // DD (a->GetID(), Telegram_ARRIVE, { target.x, target.y});
    DD (  Telegram_TOUCH_BEGIN, { ( double ) ( int ) ( &target ) } );


    static double lastClicktime = 0;
    static double nowClickTime = 0;

    nowClickTime = _dTimeNow;
    CCLOG ( "%f\n", nowClickTime - lastClicktime );
    if ( ( nowClickTime - lastClicktime ) < 0.3 )   //double click
    {
        DD (   Telegram_TOUCH_DOUBLE, { ( double ) ( int ) ( &target ) } );
    }
    lastClicktime = _dTimeNow;

    return true;
}

void HelloWorld::ccTouchMoved ( CCTouch *pTouch, CCEvent *pEvent )
{
    auto  target = pTouch->getLocation();
    DD (  Telegram_TOUCH_MOVE, { ( double ) ( int ) ( &target ) } );

}


void HelloWorld::ccTouchEnded ( CCTouch *pTouch, CCEvent *pEvent )
{
    auto  target = pTouch->getLocation();
    DD (  Telegram_TOUCH_END, { ( double ) ( int ) ( &target ) } );

}

void HelloWorld::ccTouchCancelled ( CCTouch *pTouch, CCEvent *pEvent )
{
    auto  target = pTouch->getLocation();
    DD (  Telegram_TOUCH_CANCEL, { ( double ) ( int ) ( &target ) } );

}

void HelloWorld::update ( float delta )
{
    _dTimeNow += delta;
    CCLayer::update ( delta );

    static double thinginterval = 0;
    thinginterval += delta;

    DD (Telegram_AI, { delta });
    thinginterval = 0;


    DD (Telegram_UPDATE, { delta });

}

void HelloWorld::draw (   )
{
    CCLayer::draw();

    _drawNode->clear();

    DD (Telegram_DRAW, { (double) (int)_drawNode });

    _drawNode->visit();

}

