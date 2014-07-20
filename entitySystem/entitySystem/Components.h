#pragma  once

#include "Component.h"
#include "Configuration.h"
#include "messaging/MessageDispatcher.h"
#include "cocos2d.h"
#include "ComponentMgr.h"
using namespace cocos2d;
static bool ifOutWindow (Component* c);
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

//
//class  MoveComponent :public Component
//{
//
//    vector<int>  ids_insight;
//public:
//    CCPoint  _pos;
//    CCPoint  _moveTarget;
//    CCPoint  _velocity;
//    CCPoint  _acceleration;
//    float    _maxVelocity;
//    float    _separate_threshhold;
//    float    _maxAcceleration;
//
//    static MoveComponent* C (
//
//        CCPoint  moveTarget,
//        CCPoint  velocity,
//        CCPoint  acceleration,
//        float    maxVelocity,
//        float    maxAcceleration,
//        float     maxtolerateRnage
//    )
//    {
//        return new MoveComponent (
//
//                   moveTarget,
//                   velocity,
//                   acceleration,
//                   maxVelocity,
//                   maxAcceleration,
//                   maxtolerateRnage
//
//               );
//    }
//    MoveComponent (
//
//        CCPoint  moveTarget,
//        CCPoint  velocity,
//        CCPoint  acceleration,
//        float    maxVelocity,
//        float    maxAcceleration,
//        float     separate_threshhold
//    ) :Component (Component_MOVE)
//    {
//
//        _moveTarget = moveTarget;
//        _velocity = velocity;
//        _acceleration = acceleration;
//        _maxVelocity = maxVelocity;
//        _maxAcceleration = maxAcceleration;
//        _separate_threshhold = separate_threshhold;
//
//    }
//
//    CCPoint arriveEntity (MoveComponent* m)
//    {
//
//        CCPoint vector = ccpSub (m->_pos, _pos);
//        float distance = ccpLength (vector);
//
//        float targetRadius = 5;
//        float slowRadius = targetRadius + 25;
//        static float timeToTarget = 0.1;
//
//        if (distance < targetRadius)
//        {
//            return CCPointZero;
//        }
//
//        float targetSpeed;
//        if (distance > slowRadius)
//        {
//            targetSpeed = _maxVelocity;
//        }
//        else
//        {
//            targetSpeed = _maxVelocity * distance / slowRadius;
//        }
//
//        CCPoint targetVelocity = ccpMult (ccpNormalize (vector), targetSpeed);
//
//        CCPoint acceleration = ccpMult (ccpSub (targetVelocity, _velocity), 1 / timeToTarget);
//        if (ccpLength (acceleration) > _maxAcceleration)
//        {
//            acceleration = ccpMult (ccpNormalize (acceleration), _maxAcceleration);
//        }
//        return acceleration;
//    }
//
//    CCPoint  separate()
//    {
//
//        CCPoint steering = CCPointZero;
//
//        for (int i = 0; i < ids_insight.size(); ++i)
//        {
//            auto c = CompMgr->GetComponentFromID (ids_insight[i]);
//            auto moveC = dynamic_cast<MoveComponent*> (c->GetC (Component_MOVE));
//            CCPoint direction = ccpSub (moveC->_pos, _pos);
//            float distance = ccpLength (direction);
//
//
//            if (distance < _separate_threshhold)
//            {
//                direction = ccpNormalize (direction);
//                steering =   ccpAdd (steering, ccpMult (direction, _maxAcceleration));
//            }
//        }
//        return -steering;
//    }
//
//    bool HandleMessage (const Telegram& msg)
//    {
//        switch (msg.Msg)
//        {
//        case Telegram_AI:
//        {
//
//            if ( ifOutWindow (GetParent()))return false;
//
//
//
//            DD (GetParent()->GetID(), Telegram_SEARCH, {});
//            auto pos = separate();
//            if (ccpLength (pos) == 0)
//            {
//                pos = ccp (rand() % 3 - 1, rand() % 3 - 1)*0.3f  ;
//            }
//            DD (GetParent()->GetID(), Telegram_FOLLOW_POS, { pos.x, pos.y });
//            _pos = _pos + pos;
//        }
//        break;
//        case Telegram_SET_POS:
//        {
//            _pos = ccp (msg.args[0], msg.args[1]);
//
//        }
//        break;
//        case  Telegram_SEARCH_RESULT:
//        {
//            ids_insight.clear();
//            vector<int>* lists = (vector<int>*) (size_t) (msg.args[0]);
//            for (int i = 0; i < lists->size(); ++i)
//            {
//                ids_insight.push_back ((*lists)[i]);
//            }
//        }
//        break;
//
//        default:
//            break;
//        }
//        return false;
//    }
//
//
//
//};




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
        case Telegram_DRAW:

        {
            _node->visit();
        }
        break;
        default:
            break;
        }
        return false;
    }
};


class  MoveComponent :public Component
{

    vector<int>		ids_insight;
public:
    CCPoint			_vVelocity;

    //a normalized vector pointing in the direction the entity is heading.
    CCPoint			_vHeading;
    CCPoint			_pos;
    //a vector perpendicular to the heading vector
    CCPoint			_vSide;

    double			_dMass;

    //the maximum speed this entity may travel at.
    double			_dMaxSpeed;

    //the maximum force this entity can produce to power itself
    //(think rockets and thrust)
    double			_dMaxForce;

    //the maximum rate (radians per second)this vehicle can rotate
    double			_dMaxTurnRate;
    double			_dWanderJitter;
    CCPoint			_vWanderTarget;
    double			_dWanderDistance;
    double			_dFrictionForce;
    double			_dWanderRadius;
    static MoveComponent* C (


        CCPoint			 vPos,

        double			 dMass,
        double			 dMaxSpeed,
        double			 dMaxForce,
        double			 dMaxTurnRate
    )
    {
        return new MoveComponent (


                   vPos,

                   dMass,
                   dMaxSpeed,
                   dMaxForce,
                   dMaxTurnRate
               );
    }
    MoveComponent (

        CCPoint			 vPos,

        double			 dMass,
        double			 dMaxSpeed,
        double			 dMaxForce,
        double			 dMaxTurnRate) :Component (Component_MOVE)
    {



        _pos = vPos;

        _dMass = dMass;
        _dMaxSpeed = dMaxSpeed;
        _dMaxForce = dMaxForce;
        _dMaxTurnRate = dMaxTurnRate;
        _vHeading=_vVelocity = ccp (0,0);
        _dFrictionForce=0.1;
    }

    void update (CCPoint  SteeringForce,float time_elapsed)
    {


        //Acceleration = Force/Mass

        auto  acceleration = (SteeringForce )/ _dMass;

        //update velocity

        _vVelocity = _vVelocity+ acceleration * time_elapsed;

        //make sure vehicle does not exceed maximum velocity
        if (ccpLength (_vVelocity) > _dMaxSpeed)
        {
            _vVelocity = _vVelocity.normalize()*_dMaxSpeed;
        }

        //update the position
        _pos = _pos+ _vVelocity * time_elapsed;

        //update the heading if the vehicle has a non zero velocity
        if (ccpLength (_vVelocity) > 0.00000001)
        {
            _vHeading =   _vVelocity.normalize() ;

            _vSide = ccpPerp (_vHeading);

        }
    }
    CCPoint  Seek (CCPoint TargetPos)
    {
        CCPoint DesiredVelocity =   (TargetPos - _pos).normalize()
                                    * _dMaxSpeed;

        return (DesiredVelocity - _vVelocity);
    }

    //----------------------------- Flee -------------------------------------
    //
    //  Does the opposite of Seek
    //------------------------------------------------------------------------
    CCPoint  Flee (CCPoint TargetPos)
    {
        //only flee if the target is within 'panic distance'. Work in distance
        //squared space.
        /* const double PanicDistanceSq = 100.0f * 100.0;
        if (Vec2DDistanceSq(m_pVehicle->Pos(), target) > PanicDistanceSq)
        {
        return Vector2D(0,0);
        }
        */

        CCPoint DesiredVelocity =   (_pos - TargetPos).normalize()
                                    * _dMaxSpeed;

        return (DesiredVelocity - _vVelocity);
    }

    CCPoint  Separation ( )
    {
        CCPoint SteeringForce;

        for (unsigned int i = 0; i < ids_insight.size(); ++i)
        {
            //make sure this agent isn't included in the calculations and that
            //the agent being examined is close enough. ***also make sure it doesn't
            //include the evade target ***
            auto c = CompMgr->GetComponentFromID (ids_insight[i]);
            if (!c)continue;
            auto moveC = (MoveComponent*)c->GetC (Component_MOVE);
            if (!moveC)continue;

            CCPoint ToAgent = _pos - moveC->_pos;

            //scale the force inversely proportional to the agents distance
            //from its neighbor.
            SteeringForce  = SteeringForce  +  ToAgent.normalize() / ccpLength (ToAgent);

        }

        return SteeringForce;
    }

    //Arrive makes use of these to determine how quickly a vehicle
    //should decelerate to its target
    enum Deceleration { slow = 3, normal = 2, fast = 1 };

    CCPoint  Arrive (CCPoint     TargetPos,
                     Deceleration deceleration)
    {
        CCPoint ToTarget = TargetPos - _pos ;

        //calculate the distance to the target
        double dist = ccpLength (ToTarget);

        if (dist > 0)
        {
            //because Deceleration is enumerated as an int, this value is required
            //to provide fine tweaking of the deceleration..
            const double DecelerationTweaker = 0.3;

            //calculate the speed required to reach the target given the desired
            //deceleration
            double speed = dist / ((double)deceleration * DecelerationTweaker);

            //make sure the velocity does not exceed the max
            speed = min (speed, _dMaxSpeed);

            //from here proceed just like Seek except we don't need to normalize
            //the ToTarget vector because we have already gone to the trouble
            //of calculating its length: dist.
            CCPoint DesiredVelocity = ToTarget * speed / dist;

            return (DesiredVelocity - _vVelocity);
        }

        return CCPoint (0, 0);
    }
    CCPoint  Pursuit (MoveComponent* evader)
    {
        //if the evader is ahead and facing the agent then we can just seek
        //for the evader's current position.
        CCPoint ToEvader = evader->_pos - _pos;

        double RelativeHeading = ccpDot (_vHeading ,evader->_vHeading);

        if (ccpDot (ToEvader,_vHeading) > 0  &&
                (RelativeHeading < -0.95))  //acos(0.95)=18 degs
        {
            return Seek (evader->_pos);
        }

        //Not considered ahead so we predict where the evader will be.

        //the lookahead time is propotional to the distance between the evader
        //and the pursuer; and is inversely proportional to the sum of the
        //agent's velocities
        double LookAheadTime = ccpLength (ToEvader) /
                               (_dMaxSpeed + ccpLength (_vVelocity));

        //now seek to the predicted future position of the evader
        return Seek (evader->_pos + evader->_vVelocity * LookAheadTime);
    }

    CCPoint  Evade (const MoveComponent* pursuer)
    {
        /* Not necessary to include the check for facing direction this time */

        CCPoint ToPursuer = pursuer->_pos -_pos;

        //uncomment the following two lines to have Evade only consider pursuers
        //within a 'threat range'
        const double ThreatRange = 100.0;
        if ( ccpLengthSQ (ToPursuer) > ThreatRange * ThreatRange) return CCPointZero;

        //the lookahead time is propotional to the distance between the pursuer
        //and the pursuer; and is inversely proportional to the sum of the
        //agents' velocities
        double LookAheadTime = ccpLength  (ToPursuer) /
                               (_dMaxSpeed + ccpLength (pursuer->_vVelocity));

        //now flee away from predicted future position of the pursuer
        return Flee (pursuer->_pos  + pursuer->_vVelocity  * LookAheadTime);
    }

    CCPoint   Wander (double deltaTime)
    {
        //this behavior is dependent on the update rate, so this line must
        //be included when using time independent framerate.
        double JitterThisTimeSlice = _dWanderJitter * deltaTime;

        //first, add a small random vector to the target's position
        _vWanderTarget = _vWanderTarget+ CCPoint (CCRANDOM_MINUS1_1() * JitterThisTimeSlice,
                         CCRANDOM_MINUS1_1() * JitterThisTimeSlice);

        //reproject this new vector back on to a unit circle
        _vWanderTarget=_vWanderTarget.normalize();

        //increase the length of the vector to the same as the radius
        //of the wander circle
        _vWanderTarget = _vWanderTarget* _dWanderRadius;

        //move the target into a position WanderDist in front of the agent
        CCPoint target = _vWanderTarget + CCPoint (_dWanderDistance, 0);

        assert (0);
        return CCPointZero;
        //project the target into world space
        /*CCPoint Target = PointToWorldSpace(target,
        	_vHeading,
        	_vSide,
        	_pos);
        */
        //and steer towards it
        //  return Target - _pos;
    }


    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {
        case Telegram_AI:
        {
            DD (GetParent()->GetID(), Telegram_SEARCH, {});

            update (Separation(), msg.args[0]);
            DD (GetParent()->GetID(), Telegram_FOLLOW_POS, { _pos.x, _pos.y });

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

public:
    float _range;

    static RadarComponent* C (float  range)
    {
        return  new RadarComponent (range);
    }
    RadarComponent (float  range) :Component (Component_RADAR), _range (range)
    {

    }


    bool HandleMessage (const Telegram& msg)
    {
        if (ifOutWindow (GetParent()))return false;
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
                if (c->GetParent() && c->GetID() == c->GetParent()->GetID())continue;
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
            if (targets.size()>400)
            {
                printf ("h");
            }
            DD (GetParent()->GetID(), Telegram_SEARCH_RESULT, { (double) (size_t)&targets });
        }
        break;
        case Telegram_DRAW:
        {
            MoveComponent* self = dynamic_cast<MoveComponent*> (GetParent()->GetC (Component_MOVE));

            ccDrawColor4B (122, 255, 122, 255);
            ccDrawCircle (self->_pos, _range, 0, 40, false);
            ccDrawColor4B (255, rand() % 255, rand() % 255, 255);

            for (auto a : targets)
            {

                MoveComponent* target = dynamic_cast<MoveComponent*> (CompMgr->_ComponentMap[a]->GetC (Component_MOVE));
                ccDrawLine (self->_pos, target->_pos);
            }

        }
        break;

        default:
            break;
        }
        return false;
    }
};



bool ifOutWindow (Component* c)
{

    MoveComponent* m = dynamic_cast<MoveComponent*> (c->GetC (Component_MOVE));
    //MoveComponent* m2 = dynamic_cast<MoveComponent*> (c->GetC(Component_MOVE));
    auto  _winsize = CCDirector::sharedDirector()->getWinSize();
    if (_winsize.width < (m->_pos.x) || _winsize.height < (m->_pos.y) || (m->_pos.x) < 0 || (m->_pos.y < 0))
    {
        return true;
    }
    return false;
}
