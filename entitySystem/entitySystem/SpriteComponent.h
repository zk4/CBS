//
//  SpriteComponent.h
//  entitysystem_Cocos2d-x
//
//  Created by ZK on 13-10-21.
//
//

#ifndef __entitysystem_Cocos2d_x__SpriteComponent__
#define __entitysystem_Cocos2d_x__SpriteComponent__

#include "Component.h"
#include "cocos2d.h"
USING_NS_CC;


class SpriteComponent : public Component
{
public:
    CCSprite* sprite;
    virtual ~SpriteComponent(){
        sprite->release();
    }
    SpriteComponent(string img,string id):Component(id){
        sprite=CCSprite::create(img.c_str());
        sprite->retain();
    };
    
    void     AddC(Component* c) override
    {
        assert(0);
    }
    
  
};
#endif /* defined(__entitysystem_Cocos2d_x__SpriteComponent__) */
