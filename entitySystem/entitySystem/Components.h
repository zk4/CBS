#pragma  once

#include "Component.h"
#include "Configuration.h"
#include "messaging/MessageDispatcher.h"
#include "cocos2d.h"
#include "ComponentMgr.h"
using namespace cocos2d;

class  AIComponent :public Component
{
public:
    static  AIComponent*  C()
    {
        return  new AIComponent();
    }
    AIComponent() :Component (Component_AI) { }

    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {
        /*  case AI:
          {

          DD (GetParent()->GetID(), FOLLOW_POS, { 100, 100 });
          DD (GetParent()->GetID(), ROTATE, { 1 });

          }
          break;*/

        default:
            break;
        }
        return false;
    }
};


class  MoveComponent :public Component
{

    vector<int>  ids_insight;
public:
    CCPoint  _pos;
    CCPoint  _moveTarget;
    CCPoint  _velocity;
    CCPoint  _acceleration;
    float    _maxVelocity;
    float    _maxAcceleration;

    static MoveComponent* C (

        CCPoint  moveTarget,
        CCPoint  velocity,
        CCPoint  acceleration,
        float    maxVelocity,
        float    maxAcceleration
    )
    {
        return new MoveComponent (

                   moveTarget,
                   velocity,
                   acceleration,
                   maxVelocity,
                   maxAcceleration
               );
    }
    MoveComponent (

        CCPoint  moveTarget,
        CCPoint  velocity,
        CCPoint  acceleration,
        float    maxVelocity,
        float    maxAcceleration) :Component (Component_MOVE)
    {

        _moveTarget = moveTarget;
        _velocity = velocity;
        _acceleration = acceleration;
        _maxVelocity = maxVelocity;
        _maxAcceleration = maxAcceleration;


    }

    CCPoint arriveEntity (MoveComponent* m)
    {

        CCPoint vector = ccpSub (m->_pos, _pos);
        float distance = ccpLength (vector);

        float targetRadius = 5;
        float slowRadius = targetRadius + 25;
        static float timeToTarget = 0.1;

        if (distance < targetRadius)
        {
            return CCPointZero;
        }

        float targetSpeed;
        if (distance > slowRadius)
        {
            targetSpeed = _maxVelocity;
        }
        else
        {
            targetSpeed = _maxVelocity * distance / slowRadius;
        }

        CCPoint targetVelocity = ccpMult (ccpNormalize (vector), targetSpeed);

        CCPoint acceleration = ccpMult (ccpSub (targetVelocity, _velocity), 1 / timeToTarget);
        if (ccpLength (acceleration) > _maxAcceleration)
        {
            acceleration = ccpMult (ccpNormalize (acceleration), _maxAcceleration);
        }
        return acceleration;
    }

    CCPoint  separate()
    {

        CCPoint steering = CCPointZero;

        for (int i = 0; i < ids_insight.size(); ++i)
        {
            auto c = CompMgr->GetComponentFromID (ids_insight[i]);
            auto target = dynamic_cast<MoveComponent*> (c->GetC (Component_MOVE));
            CCPoint direction = ccpSub (target->_pos, _pos);
            float distance = ccpLength (direction);
            static float SEPARATE_THRESHHOLD = 20;

            if (distance < SEPARATE_THRESHHOLD)
            {
                direction = ccpNormalize (direction);
                steering = ccpAdd (steering, ccpMult (direction, _maxAcceleration));
            }
        }
        return steering;
    }

    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {
        case Telegram_AI:
        {
            //  DD (GetParent()->GetID(), SEARCH, {});
            auto pos = separate();
            DD (GetParent()->GetID(), Telegram_FOLLOW_POS, { pos.x, pos.y });
        }
        break;
        case Telegram_SET_POS:
        {
            _pos = ccp (msg.args[0], msg.args[1]);
        }
        break;
        case  Telegram_SEARCH_RESULT:
        {
            ids_insight.clear();
            vector<int>* lists = (vector<int>*) (size_t) (msg.args[0]);
            for (int i = 0; i < lists->size(); ++i)
            {
                ids_insight.push_back ((*lists)[i]);
            }
        }
        break;

        default:
            break;
        }
        return false;
    }

};
class  RadarComponent :public Component
{
    vector< int >  targets;
    float _range;
public:

    static RadarComponent* C (float  range)
    {
        return  new RadarComponent (range);
    }
    RadarComponent (float  range) :Component (Component_RADAR), _range (range) { }


    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {

        case Telegram_SEARCH:
        {
            MoveComponent* self = dynamic_cast<MoveComponent*> (GetParent()->GetC (Component_MOVE));
            targets.clear();
            for (int i = 0; i < CompMgr->_ComponentMap.size(); ++i)
            {
                auto c = CompMgr->_ComponentMap[i];
                if (!c)continue;
                MoveComponent* target = dynamic_cast<MoveComponent*> (c->GetC (Component_MOVE));
                if (!target)continue;
                if (target->GetParent() != GetParent())
                {

                    if (ccpLength (ccpSub (target->_pos, self->_pos)) < _range)
                    {
                        targets.push_back (target->GetParent()->GetID());
                    }
                }
            }
            DD (GetParent()->GetID(), Telegram_SEARCH_RESULT, { (double) (size_t)&targets });
        }
        break;

        default:
            break;
        }
        return false;
    }
};






class WeaponComponent : public Component
{

public:
    WeaponComponent() :Component (Component_WEAPON)
    {
    }
    static  WeaponComponent* Create()
    {
        return new WeaponComponent();
    }

    bool HandleMessage (const Telegram& msg)
    {
        return false;
    }
    void Attack (int id, double hp)
    {
        Dispatcher->DispatchMessageZZ (0, GetID(), id, Telegram_HURT, { hp });
    }
};




class HPComponent : public Component
{
    int HP;

public:
    HPComponent (int hp) :Component (Component_HP), HP (hp)
    {
    }
    static  HPComponent* Create (int hp)
    {
        return  new HPComponent (hp);
    }
    bool  HandleMessage (const Telegram& msg)
    {
        if (msg.Msg == Telegram_HURT)
        {
            HP -= (int)msg.args[0];
            if (HP < 0)
            {
                DD (0, GetID(), GetParent()->GetID(), Telegram_DEAD, { 0 });
            }
        }

        return false;
    }

};

class  CocosComponent :public Component
{
public:
    CCNode*   _node;
    inline CCNode* Node()
    {
        return _node;
    };
    static  CocosComponent* C (CCNode* node)
    {
        return  new CocosComponent (node);
    }
    ~CocosComponent()
    {
        _node->release();
    }
    CocosComponent (CCNode* node) :Component (Component_COCOS)
    {
        _node = CCSprite::create();
        _node->retain();
        _node->addChild (node);
    }
    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {
        case Telegram_SET_POS:
        {
            _node->setPosition (ccp (msg.args[0], msg.args[1]));
        }
        break;
        case Telegram_MOVETO_POS:
        {
            _node->stopActionByTag (Telegram_MOVETO_POS);

            auto act = CCMoveTo::create (msg.args[0], ccp (msg.args[1], msg.args[2]));
            act->setTag (Telegram_MOVETO_POS);
            _node->runAction (act);
        }
        break;
        case Telegram_FOLLOW_POS:
        {

            auto touch = ccp (msg.args[0], msg.args[1]);


            _node->setPosition (_node->getPosition() + touch);
        }
        break;
        case Telegram_ROTATE:
        {

            _node->setRotation (_node->getRotation() + msg.args[0]);
        }
        break;
        default:
            break;
        }
        return false;
    }
};