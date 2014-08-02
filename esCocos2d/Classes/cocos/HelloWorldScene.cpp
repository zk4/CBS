#include "HelloWorldScene.h"


#include "Components.h"
USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild (layer);

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
    _winsize=CCDirector::sharedDirector()->getWinSize();
    // Create batch node
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile ("Sprites.plist");
    _batchNode = CCSpriteBatchNode::create ("Sprites.pvr.ccz");
    addChild ( _batchNode);

    // bg = Component::Create (Entity_);
    // bg->AddC(SpriteComponent::Create(CCSprite::create("worldmap.png")));
    // bg->AddC (MoveComponent::Create ({ 100, 100 }, 1, 300, 300, 100,1));
    // DD(bg->GetID(), Telegram_SET_POS, { visibleSize.width / 2, visibleSize.height / 2 });


    wall = Component::Create (Entity_);
    wall->AddC (WallComponents::Create (50));
    wall->AddC (MoveComponent::Create ({ 100, 100 }, 1, 300, 300, 100, 1));
    DD (wall->GetID(), Telegram_SET_POS, { 0,0 });


    srand (time (NULL));

    auto _rt = CCRenderTexture::create (_winsize.width, _winsize.height, kCCTexture2DPixelFormat_RGBA8888);
    _rt->setPosition (_winsize/2 );
    _rt->retain();

    for (int i=0; i<0; ++i)
    {

        auto   planes = Component::Create (Entity_);
        auto sprite = CCSprite::create ("airplane.png");
        sprite->setScale (.05f);
        CCMenuItemImage* cc = CCMenuItemImage::create ("CloseNormal.png", "CloseSelected.png", "CloseSelected.png", this, menu_selector (HelloWorld::menuCloseCallback));
        CCMenu* c = CCMenu::create (cc,NULL);
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate (c, 0,false);
        sprite->addChild (c);
        sprite->setPosition (CCPointZero);
        cc->setPosition (CCPointZero);
        c->setPosition (CCPointZero);
        planes->AddC (TrailComponent::Create (_rt));
        planes->AddC (RadarComponent::Create (50));
        planes->AddC (MoveComponent::Create ({ 100, 100 }, 1,100, 1000, 100,1));
        planes->AddC (SpriteComponent::Create (sprite));

        planes->AddC (HPComponent::Create (100));
        planes->AddC (WeaponComponent::Create());
        bads.push_back (planes);

        DD (planes->GetID(), Telegram_SET_POS, { double (200 + rand() % 30), double (200 + rand() % 30) });
    }
    for (auto a:bads)
    {
        DD (a->GetID(), Telegram_SEARCH, {});
    }

    setTouchEnabled (true);
    setTouchMode (kCCTouchesOneByOne);
    scheduleUpdate();
    return true;
}


void HelloWorld::menuCloseCallback (CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit (0);
#endif
}

static bool touched=true;
static CCPoint target=CCPointZero;
bool HelloWorld::ccTouchBegan (CCTouch *pTouch, CCEvent *pEvent)
{
    touched=true;
    target=pTouch->getLocation();

    //  auto a = bads[0];
    /*  {

          MoveComponent* m = dynamic_cast<MoveComponent*> (a->GetC (Component_MOVE));

          m->_pos = target;
      }*/
    // DD (a->GetID(), Telegram_ARRIVE, { target.x, target.y});
    DD (wall->GetID(), Telegram_CLIK_ON_WALL, { target.x, target.y });

    return true;
}

void HelloWorld::ccTouchMoved (CCTouch *pTouch, CCEvent *pEvent)
{
    target = pTouch->getLocation();
    DD (wall->GetID(), Telegram_CLIK_ON_WALL, { target.x, target.y });
}


void HelloWorld::ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent)
{
    touched=false;
}

void HelloWorld::ccTouchCancelled (CCTouch *pTouch, CCEvent *pEvent)
{
    touched = false;
}

void HelloWorld::update (float delta)
{
    CCLayer::update (delta);

    static double thinginterval =0;
    thinginterval+= delta;

    for (int i=0; i < bads.size(); ++i)
    {
        DD (bads[i]->GetID(), Telegram_AI, {delta});
        DD (bads[i]->GetID(), Telegram_UPDATE, { delta });
    }
    thinginterval=0;
    DD (wall->GetID(), Telegram_UPDATE, { delta });

}

void HelloWorld::draw()
{
    CCLayer::draw();

//    DD (bg->GetID(), Telegram_DRAW, {});
    for (int i = 0; i < bads.size(); ++i)
    {

        DD (bads[i]->GetID(), Telegram_DRAW, {});
    }
    DD (wall->GetID(), Telegram_DRAW, {});

}

