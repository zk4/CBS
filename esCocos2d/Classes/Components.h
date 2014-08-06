#pragma  once

#include "es/Component.h"
#include "Configuration.h"
#include "messaging/MessageDispatcher.h"
#include "cocos2d.h"
#include "es/ComponentMgr.h"
#include "algorithm/Path.h"
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <iterator>
#include <unordered_map>
using namespace cocos2d;
static bool ifOutWindow (Component* c);


class SpriteComponent :public Component
{
public:
    CCSprite*   _sprite;
    inline CCNode* Node()
    {
        return _sprite;
    };
    static  SpriteComponent* Create (CCSprite* c)
    {
        return  new SpriteComponent (c);
    }
    ~SpriteComponent()
    {

        _sprite->release();
    }
    SpriteComponent (CCSprite* c) :Component (Component_SPRITE)
    {
        _sprite = c;
        _sprite->retain();

    }

    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {
        case Telegram_SET_POS:
        {
            _sprite->setPosition (ccp (msg.args[0], msg.args[1]));
        }
        break;
        case Telegram_SET_ROTATION:
        {
            _sprite->setRotation (CC_RADIANS_TO_DEGREES (ccpToAngle (ccp (msg.args[0], msg.args[1]))));

        }
        break;
        case Telegram_SET_SCALE:
        {
            _sprite->setScaleX (msg.args[0]);
        }
        break;
        case  Telegram_SET_CONTENSIZE:
        {
            _sprite->setContentSize (CCSizeMake (msg.args[0], msg.args[1]));
        }
        break;
        case Telegram_DRAW:

        {
            _sprite->visit();
        }
        break;
        default:
            break;
        }
        return false;
    }
};

class  CocosComponent :public Component
{
public:
    CCLayer*   _delegate;
    inline CCNode* Node()
    {
        return _delegate;
    };
    static  CocosComponent* Create (CCNode* node)
    {
        return  new CocosComponent (node);
    }
    ~CocosComponent()
    {

        _delegate->release();
    }
    CocosComponent (CCNode* node) :Component (Component_COCOS)
    {
        _delegate = CCLayer::create();

        _delegate->retain();
        _delegate->addChild (node);
    }
    void addChild (CCNode* c)
    {
        _delegate->addChild (c);

    }
    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {
        case Telegram_SET_POS:
        {
            _delegate->setPosition (ccp (msg.args[0], msg.args[1]));
        }
        break;
        case Telegram_SET_ROTATION:
        {
            _delegate->setRotation (acosf (ccp (0, 1).cross (ccp (msg.args[0], msg.args[1]).normalize())));
        }


        case Telegram_DRAW:

        {
            _delegate->visit();
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
    double			_dFrictionMu;
    double			_dWanderRadius;
    double			 _dBoundingRadius;
    CCPoint		_vSteeringForce;
    static MoveComponent* Create (


        CCPoint			 vPos,

        double			 dMass,
        double			 dMaxSpeed,
        double			 dMaxForce,
        double			 dMaxTurnRate,
        double			 dFriction��
    )
    {
        return new MoveComponent (
                   vPos,
                   dMass,
                   dMaxSpeed,
                   dMaxForce,
                   dMaxTurnRate,
                   dFriction��
               );
    }


    void update (CCPoint  SteeringForce, float time_elapsed)
    {


        //Acceleration = Force/Mass
        CCPoint friction = (ccpLength (_vVelocity) > 0) ? -_vVelocity.normalize()*_dFrictionMu*_dMass : CCPointZero;


        SteeringForce = friction + SteeringForce;

        auto  acceleration = SteeringForce / _dMass;


        //update velocity

        _vVelocity = _vVelocity + acceleration * time_elapsed;

        //make sure vehicle does not exceed maximum velocity
        if (ccpLength (_vVelocity) > _dMaxSpeed)
        {
            _vVelocity = _vVelocity.normalize()*_dMaxSpeed;
        }

        //update the position
        _pos = _pos + _vVelocity * time_elapsed;

        CCSize size = CCDirector::sharedDirector()->getWinSize();
        if (_pos.x<0 || _pos.x>size.width)
            _vVelocity.x = -_vVelocity.x;

        if (_pos.y<0 || _pos.y>size.height)
            _vVelocity.y = -_vVelocity.y;



        //update the heading if the vehicle has a non zero velocity
        if (ccpLength (_vVelocity) > 0.00000001)
        {
            _vHeading = _vVelocity.normalize();
            _vSide = ccpPerp (_vHeading);

        }
    }
    CCPoint  Seek (CCPoint TargetPos)
    {
        CCPoint DesiredVelocity = (TargetPos - _pos).normalize()
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

        CCPoint DesiredVelocity = (_pos - TargetPos).normalize()
                                  * _dMaxSpeed;

        return (DesiredVelocity - _vVelocity);
    }

    //-------------------------------- Cohesion ------------------------------
    //
    //  returns a steering force that attempts to move the agent towards the
    //  center of mass of the agents in its immediate area
    //------------------------------------------------------------------------
    CCPoint Cohesion (const vector<MoveComponent*> &neighbors)
    {
        //first find the center of mass of all the agents
        CCPoint CenterOfMass, SteeringForce;

        int NeighborCount = 0;

        //iterate through the neighbors and sum up all the position vectors
        for (unsigned int a = 0; a < neighbors.size(); ++a)
        {
            //make sure *this* agent isn't included in the calculations and that
            //the agent being examined is close enough ***also make sure it doesn't
            //include the evade target ***


            CenterOfMass = CenterOfMass + neighbors[a]->_pos;

            ++NeighborCount;

        }

        if (NeighborCount > 0)
        {
            //the center of mass is the average of the sum of positions
            CenterOfMass = CenterOfMass / (double)NeighborCount;

            //now seek towards that position
            SteeringForce = Seek (CenterOfMass);
        }

        //the magnitude of cohesion is usually much larger than separation or
        //allignment so it usually helps to normalize it.
        return    SteeringForce.normalize();
    }
    CCPoint  Separation()
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
            SteeringForce = SteeringForce + ToAgent.normalize() *moveC->_dMaxForce;

        }

        return SteeringForce;
    }

    //Arrive makes use of these to determine how quickly a vehicle
    //should decelerate to its target
    enum Deceleration { slow = 3, normal = 2, fast = 1 };

    CCPoint  Arrive (CCPoint     TargetPos,
                     Deceleration deceleration)
    {
        CCPoint ToTarget = TargetPos - _pos;

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

        double RelativeHeading = ccpDot (_vHeading, evader->_vHeading);

        if (ccpDot (ToEvader, _vHeading) > 0 &&
                (RelativeHeading < -0.95))   //acos(0.95)=18 degs
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

    //--------------------- PointToWorldSpace --------------------------------
    //
    //  Transforms a point from the agent's local space into world space
    //------------------------------------------------------------------------
    inline CCPoint PointToWorldSpace (const CCPoint &point,
                                      const CCPoint &AgentHeading,
                                      const CCPoint &AgentSide,
                                      const CCPoint &AgentPosition)
    {
        //make a copy of the point
        CCPoint TransPoint = point;

        //create a transformation matrix
        CCAffineTransform matTransform;

        //rotate
        //matTransform.Rotate(AgentHeading, AgentSide);
        auto ac = AgentHeading.normalize();
        auto bd = AgentSide.normalize();
        matTransform.a = ac.x;
        matTransform.c = ac.y;
        matTransform.b = bd.x;
        matTransform.d = bd.x;

        //and translate

        CCAffineTransformTranslate (matTransform, AgentPosition.x, AgentPosition.y);
        //now transform the vertices
        TransPoint = CCPointApplyAffineTransform (TransPoint, matTransform);
        return TransPoint;
    }
    CCPoint  Evade (const MoveComponent* pursuer)
    {
        /* Not necessary to include the check for facing direction this time */

        CCPoint ToPursuer = pursuer->_pos - _pos;

        //uncomment the following two lines to have Evade only consider pursuers
        //within a 'threat range'
        const double ThreatRange = 100.0;
        if (ccpLengthSQ (ToPursuer) > ThreatRange * ThreatRange) return CCPointZero;

        //the lookahead time is propotional to the distance between the pursuer
        //and the pursuer; and is inversely proportional to the sum of the
        //agents' velocities
        double LookAheadTime = ccpLength (ToPursuer) /
                               (_dMaxSpeed + ccpLength (pursuer->_vVelocity));

        //now flee away from predicted future position of the pursuer
        return Flee (pursuer->_pos + pursuer->_vVelocity  * LookAheadTime);
    }

    CCPoint   Wander (double deltaTime)
    {
        //this behavior is dependent on the update rate, so this line must
        //be included when using time independent framerate.


        //first, add a small random vector to the target's position
        _vWanderTarget = _vWanderTarget + CCPoint (CCRANDOM_MINUS1_1(),
                         CCRANDOM_MINUS1_1());

        //reproject this new vector back on to a unit circle
        _vWanderTarget = _vWanderTarget.normalize();

        //increase the length of the vector to the same as the radius
        //of the wander circle
        _vWanderTarget = _vWanderTarget* _dWanderRadius;

        return _vWanderTarget;

    }
    CCPoint  Interpose (const MoveComponent* AgentA,
                        const MoveComponent* AgentB)
    {
        //first we need to figure out where the two agents are going to be at
        //time T in the future. This is approximated by determining the time
        //taken to reach the mid way point at the current time at at max speed.
        CCPoint MidPoint = (AgentA->_pos + AgentB->_pos) / 2.0;

        double TimeToReachMidPoint = ccpDistance (_pos, MidPoint) /
                                     _dMaxSpeed;

        //now we have T, we assume that agent A and agent B will continue on a
        //straight trajectory and extrapolate to get their future positions
        CCPoint APos = AgentA->_pos + AgentA->_vVelocity * TimeToReachMidPoint;
        CCPoint BPos = AgentB->_pos + AgentB->_vVelocity * TimeToReachMidPoint;

        //calculate the mid point of these predicted positions
        MidPoint = (APos + BPos) / 2.0;

        //then steer to Arrive at it
        return Arrive (MidPoint, fast);
    }

    //--------------------------- Hide ---------------------------------------
    //
    //------------------------------------------------------------------------
    CCPoint  Hide (const MoveComponent*           hunter,
                   const vector<MoveComponent*>&  obstacles)
    {
        double    DistToClosest = (std::numeric_limits<double>::max)();
        CCPoint BestHidingSpot;

        std::vector<MoveComponent*>::const_iterator curOb = obstacles.begin();
        std::vector<MoveComponent*>::const_iterator closest;

        while (curOb != obstacles.end())
        {
            //calculate the position of the hiding spot for this obstacle
            CCPoint HidingSpot = GetHidingPosition ((*curOb)->_pos,
                                                    (*curOb)->_dBoundingRadius,
                                                    hunter->_pos);

            //work in distance-squared space to find the closest hiding
            //spot to the agent
            double dist = ccpDistanceSQ (HidingSpot, _pos);

            if (dist < DistToClosest)
            {
                DistToClosest = dist;

                BestHidingSpot = HidingSpot;

                closest = curOb;
            }

            ++curOb;

        }//end while

        //if no suitable obstacles found then Evade the hunter
        if (DistToClosest == (std::numeric_limits<float>::max)())
        {
            return Evade (hunter);
        }

        //else use Arrive on the hiding spot
        return Arrive (BestHidingSpot, fast);
    }
    CCPoint  GetHidingPosition (const CCPoint& posOb,
                                const double     radiusOb,
                                const CCPoint& posHunter)
    {
        //calculate how far away the agent is to be from the chosen obstacle's
        //bounding radius
        const double DistanceFromBoundary = 30.0;
        double       DistAway = radiusOb + DistanceFromBoundary;

        //calculate the heading toward the object from the hunter
        CCPoint ToOb = (posOb - posHunter).normalize();

        //scale it to size and add to the obstacles position to get
        //the hiding spot.
        return (ToOb * DistAway) + posOb;
    }


    //---------------------------- Alignment ---------------------------------
    //
    //  returns a force that attempts to align this agents heading with that
    //  of its neighbors
    //------------------------------------------------------------------------
    CCPoint Alignment (const vector<MoveComponent*>& neighbors)
    {
        //used to record the average heading of the neighbors
        CCPoint AverageHeading;

        //used to count the number of vehicles in the neighborhood
        int    NeighborCount = 0;

        //iterate through all the tagged vehicles and sum their heading vectors
        for (unsigned int a = 0; a < neighbors.size(); ++a)
        {
            //make sure *this* agent isn't included in the calculations and that
            //the agent being examined  is close enough ***also make sure it doesn't
            //include any evade target ***

            {
                AverageHeading = AverageHeading + neighbors[a]->_vHeading;

                ++NeighborCount;
            }
        }

        //if the neighborhood contained one or more vehicles, average their
        //heading vectors.
        if (NeighborCount > 0)
        {
            AverageHeading = AverageHeading / (double)NeighborCount;

            AverageHeading = AverageHeading - _vHeading;
        }

        return AverageHeading;
    }

    MoveComponent (

        CCPoint			 vPos,

        double			 dMass,
        double			 dMaxSpeed,
        double			 dMaxForce,
        double			 dMaxTurnRate,
        double			dFrictionMu) :Component (Component_MOVE)
    {
        _pos = vPos;
        _dMass = dMass;
        _dMaxSpeed = dMaxSpeed;
        _dMaxForce = dMaxForce;
        _dMaxTurnRate = dMaxTurnRate;
        _vHeading = _vVelocity = ccp (0, 0);
        _dFrictionMu = dFrictionMu;
        _dWanderJitter = 1;
        _dWanderRadius = 10;
        _dWanderDistance = 10;
        ttf_velocity = CCLabelTTF::create();
        ttf_velocity->setFontSize (32);
        ttf_velocity->setString ("V");
        ttf_velocity->retain();

        ttf_head = CCLabelTTF::create();
        ttf_head->setFontSize (32);
        ttf_head->setString ("H");
        ttf_head->retain();
        ttf_head->setColor (ccc3 (255, 0, 255));

    }
    CCLabelTTF* ttf_velocity;
    CCLabelTTF* ttf_head;
    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {
        case Telegram_DRAW:
        {
            if (ccpLength (_vVelocity) > 0.5)
            {
                ccDrawColor4B (255, 255, 255, 255);
                ccDrawLine (_pos, _pos + _vVelocity);
                ttf_velocity->setPosition (_pos + _vVelocity / 2);
                ttf_velocity->visit();

                ccDrawColor4B (255, 0, 255, 255);
                ccDrawLine (_pos, _pos + _vHeading.normalize() * 50);
                ttf_head->setPosition (_pos + _vHeading.normalize() * 50 / 2);
                ttf_head->visit();
            }

        }
        break;
        case Telegram_UPDATE:
        {
            _vSteeringForce = Wander (msg.args[0]);
            update (_vSteeringForce, msg.args[0]);
            DD (GetParent()->GetID(), Telegram_SET_POS, { _pos.x, _pos.y });
            DD (GetParent()->GetID(), Telegram_SET_ROTATION, { _vVelocity.y, _vVelocity.x });
        }
        break;
        case Telegram_ARRIVE:
        {
            DD (GetParent()->GetID(), Telegram_SEARCH, {});
            _vSteeringForce = Arrive (ccp (msg.args[0], msg.args[1]), fast);
            _vHeading = ccp (msg.args[0], msg.args[1]) - _pos;
            update (_vSteeringForce, 0.1);


        }
        break;
        case Telegram_AI:
        {
            DD (GetParent()->GetID(), Telegram_SEARCH, {});


            //	 _vSteeringForce = Wander();



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

    static RadarComponent* Create (float  range)
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


class HPComponent : public Component
{


public:
    int HP;
    HPComponent (int hp) :Component (Component_HP), HP (hp)
    {
    }
    static  HPComponent* Create (int hp)
    {
        return  new HPComponent (hp);
    }
    bool  HandleMessage (const Telegram& msg)
    {

        switch (msg.Msg)
        {
        case Telegram_HURT:
        {
            HP -= (int)msg.args[0];
            if (HP < 0)
            {
                HP = 0;
                DD (Telegram_DEAD, { (double)GetParent()->GetID() });
            }
        }
        break;
        case Telegram_DRAW:
        {
            auto moveC = (MoveComponent*)GetParent()->GetC (Component_MOVE);
            ccDrawSolidRect (moveC->_pos + ccp (-HP / 2, 40), moveC->_pos + ccp (HP / 2, 45), { 255, 0, 0, 122 });
        }
        break;

        default:
            break;
        }

        return false;
    }

};


class TrailComponent : public Component
{

public:
    CCRenderTexture*  _rt;
    CCSprite*    _pBrush;
    TrailComponent (CCRenderTexture*  rt) :Component (Component_TRAIL), _rt (rt)
    {
        _pBrush = CCSprite::create ("stroke.png");
        _pBrush->retain();
        _pBrush->setColor (ccRED);
        _pBrush->setOpacity (20);
    }
    ~TrailComponent()
    {
        _pBrush->release();
    }
    static  TrailComponent* Create (CCRenderTexture*  rt)
    {
        return new TrailComponent (rt);
    }

    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {
        case Telegram_SET_POS:
        {

            auto moveC = (MoveComponent*)GetParent()->GetC (Component_MOVE);
            CCPoint start = ccp (msg.args[0], msg.args[1]);


            _rt->begin();

            _pBrush->setPosition (start);
            _pBrush->setRotation (rand() % 360);
            float r = (float) (rand() % 3 / 20.f);
            _pBrush->setScale (r);

            _pBrush->setColor (ccc3 (rand() % 127 + 128, 255, 0));

            _pBrush->visit();

            _rt->end();
        }
        break;
        case Telegram_DRAW:
        {
            _rt->visit();
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
    vector< int >  ids_insight;
    CCPoint  _ccpTarget;
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
        switch (msg.Msg)
        {
        case Telegram_AI:
        {
            if (ids_insight.size() > 0)
            {
                Component* a = NULL;
                for (auto c : ids_insight)
                {

                    auto hpC = (HPComponent*)CompMgr->GetComponentFromID (c)->GetC (Component_HP);
                    if (hpC && hpC->HP > 0)
                    {
                        a = CompMgr->GetComponentFromID (c);
                        break;
                    }
                }
                if (!a)return false;

                auto moveC = (MoveComponent*)a->GetC (Component_MOVE);
                if (moveC)
                {
                    _ccpTarget = moveC->_pos;
                    DD (a->GetID(), Telegram_ARRIVE, { moveC->_pos.x, moveC->_pos.y });
                    DD (a->GetID(), Telegram_HURT, { 1 });
                }
            }

        }
        break;
        case     Telegram_DRAW:
        {
            /*    auto moveC = (MoveComponent*)GetParent()->GetC (Component_MOVE);

              if (moveC)
              {
              ccDrawColor4B (122, 122, 255, 255);
              ccDrawLine (moveC->_pos, _ccpTarget);
              }*/
        }
        break;
        case  Telegram_SEARCH_RESULT:
        {
            if (ids_insight.empty())
            {
                vector<int>* lists = (vector<int>*) (size_t) (msg.args[0]);
                for (int i = 0; i < lists->size(); ++i)
                {
                    ids_insight.push_back ((*lists)[i]);
                }
            }

        }
        break;

        default:
            break;
        }
        return false;

    }
    void Attack (int id, double hp)
    {
        DD (0,  GetID(), id, Telegram_HURT, { hp });
    }
};
#define PTM_RATIO 32.0
#include "Box2D/Box2D.h"

class Box2DComponent :public Component
{
public:
    b2World *_world;
    b2Body *_body;
    CCSprite *_ball;
    static  Box2DComponent* Create()
    {
        return new Box2DComponent();
    }
    ~Box2DComponent()
    {
        _ball->release();
    }
    Box2DComponent() :Component (Component_BOX2D)
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        // Create sprite and add it to the layer
        _ball = CCSprite::create ("1.png");
        _ball->setPosition (winSize / 2);
        _ball->retain();

        // Create a world
        b2Vec2 gravity = b2Vec2 (0.0f, -10.0f);
        bool doSleep = true;
        _world = new b2World (gravity);

        // Create edges around the entire screen
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set (0, 0);
        b2Body *groundBody = _world->CreateBody (&groundBodyDef);
        b2PolygonShape groundBox;
        b2FixtureDef boxShapeDef;
        boxShapeDef.shape = &groundBox;

        groundBox.SetAsBox (4, winSize.height / PTM_RATIO / 2);
        groundBody->CreateFixture (&boxShapeDef);


        // Create ball body and shape
        b2BodyDef ballBodyDef;
        ballBodyDef.type = b2_dynamicBody;
        ballBodyDef.position.Set (winSize.width / 2 / PTM_RATIO, winSize.height / 2 / PTM_RATIO);
        ballBodyDef.userData = _ball;
        _body = _world->CreateBody (&ballBodyDef);

        b2CircleShape circle;
        circle.m_radius = 26.0 / PTM_RATIO;

        b2FixtureDef ballShapeDef;
        ballShapeDef.shape = &circle;
        ballShapeDef.density = 1.0f;
        ballShapeDef.friction = 0.2f;
        ballShapeDef.restitution = 0.8f;
        _body->CreateFixture (&ballShapeDef);


    }
    void tick (float dt)
    {

        _world->Step (dt, 10, 10);
        for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext())
        {
            if (b->GetUserData() != NULL)
            {
                CCSprite *ballData = (CCSprite *)b->GetUserData();
                ballData->setPosition (ccp (b->GetPosition().x * PTM_RATIO,
                                            b->GetPosition().y * PTM_RATIO));
                ballData->setRotation (-1 * CC_RADIANS_TO_DEGREES (b->GetAngle()));
            }
        }

    }
    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {

        case    Telegram_UPDATE:
        {
            tick (msg.args[0]);
        }
        break;
        case    Telegram_DRAW:
        {
            _ball->visit();
        }
        break;

        default:
            break;
        }
        return false;

    }
};


bool operator== (const CCPoint& p1, const CCPoint& p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

bool operator!= (const CCPoint& p1, const CCPoint& p2)
{
    return  ! (p1==p2);
}
bool operator< (const CCPoint& p1, const CCPoint& p2)
{
    return p1.x<p2.x || (p1.x==p2.x && p1.y<p2.y);
}


class WallComponents :public Component
{
    class SetComp
    {
    public:
        int operator() (const CCPoint& p1, const CCPoint& p2)
        {
            return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
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
    struct ccp_ext
    {
        CCPoint  p;
        int      iAccessTimes;
        CCPoint  direction;
    };
    vector<ccp_ext>			    _access_nodes;

public:
    static  WallComponents* Create (int width)
    {
        return new WallComponents (width);
    }
    ~WallComponents()
    {

    }



    void setWall (CCPoint& p)
    {
        _graph.SetNodeValidate (p, false);
        auto ite = find_if (_nodes.begin(), _nodes.end(), [&p] (CCPoint& pp)
        {
            return p ==pp;
        });
        if (ite==_nodes.end())
            _nodes.push_back (p);
    }
    WallComponents (int w) :Component (Component_BOX2D), _width (w), _thread (std::bind (&WallComponents::construct, this))
    {
        _ccp_RL = ccp (50, 50);
        MakeGraph (_ccp_RL);
        _access_count = CCLabelTTF::create ("", "Helvetica", 16);
        _access_count->retain();
        _start = ccp (5,5);
        _end= CCPointZero;
        _last_ite = -1;
        _bChanged=false;

    }

    void  MakeGraph (CCPoint  right_left)
    {
        _graph.clear();


        for (int x = 0; x <= right_left.x; ++x)
        {

            for (int y = 0; y <= right_left.y; ++y)
            {

                CCPoint p = ccp (x, y);
                CCPoint p4[] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } /* , { 1,1 }, { -1, 1 }, { -1, -1 }, { 1, -1 }   */};
                for (auto& go : p4)
                {

                    CCPoint possibale = p + go;


                    _graph.addNEdge ( (p),   (possibale), go.getLength());

                }
            }

        }


    }

    void construct()
    {

        while (true)
        {
            std::unique_lock<std::mutex> lk (_cv_m);
            CCPoint oldend = _end;
            CCPoint oldstart= _start;
            _cv.wait (lk, [=] {return  oldend != _end || _start != oldstart || _bChanged==true; });

            _bChanged=false;

            _shorest.clear();

            auto from = _graph.findNode (_start);
            auto  current = _graph.findNode (_end);


            {
                _graph.AStar (from , current , [] (const CCPoint& a, const CCPoint& b)
                {
                    return  a.getDistance (b)*1.2;
                }       );
            }



            while (current && current != from)
            {
                _shorest.push_back (current->data);
                current = current->p;
            }

            _cv.notify_all();
        }

    }

    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {

        case    Telegram_UPDATE:
        {

        }
        break;
        case Telegram_ACCESS_NODE:
        {

            CCPoint * world_pos = reinterpret_cast<CCPoint*> ((int) (msg.args[0]));
            //  CCPoint * world_parent_pos = reinterpret_cast<CCPoint*> ((int) (msg.args[1]));
            CCPoint  direction = *world_pos;// ccpSub(*world_parent_pos, *world_pos).normalize();
            auto  ite = find_if (_access_nodes.begin(), _access_nodes.end(), [=] (const  ccp_ext & ce)
            {
                return ce.p == *world_pos;
            });
            if (ite != _access_nodes.end())
            {
                ite->iAccessTimes++;
                ite->direction=direction;
            }
            else
            {
                _access_nodes.push_back ({ *world_pos, 1 });
                _access_nodes.rbegin()->direction = direction;
            }
        }
        break;
        case  Telegram_TOUCH_DOUBLE:
        {
            CCPoint * world_pos = reinterpret_cast<CCPoint*> ((int) (msg.args[0]));

            for (auto a =_nodes.begin(); a!=_nodes.end(); ++a)
            {
                CCRect r = CCRectMake (a->x*_width, a->y*_width, _width, _width);
                if (r.containsPoint (*world_pos))
                {
                    _graph.SetNodeValidate (*a,true);
                    _nodes.erase (a);
                    break;
                }
            }
            _bChanged = true;
            _cv.notify_all();
        }
        break;
        case Telegram_TOUCH_MOVE:
        {
            CCPoint * world_pos = reinterpret_cast<CCPoint*> ((int) (msg.args[0]));
            int x = world_pos->x / _width;
            int y = world_pos->y / _width;
            if (_last_ite == WallComponents_start)
            {
                _start = ccp (x, y);
            }
            else if (_last_ite == WallComponents_end)
            {
                _end = ccp (x, y);
            }
            else if (_last_ite != WallComponents_none)
            {
                if (_nodes.size()>_last_ite)
                {
                    _graph.SetNodeValidate (_nodes[_last_ite], true);
                    _nodes[_last_ite] = ccp (x,y);
                    setWall (ccp (x, y));
                }

            }
            else
            {
                setWall (ccp (x, y));
            }
            _bChanged=true;
            _cv.notify_all();

        }
        break;
        case Telegram_TOUCH_BEGIN:
        {

            CCPoint * world_pos = reinterpret_cast<CCPoint*> ((int) (msg.args[0]));

            int x = world_pos->x / _width;
            int y = world_pos->y / _width;

            _last_ite =WallComponents_none;
            for (int i=0; i <_nodes.size(); ++i)
            {
                if (ccp (x,y) == _nodes[i])
                {
                    _last_ite=i;
                    break;
                }
            }
            {
                CCRect r = CCRectMake (_start.x*_width, _start.y*_width, _width, _width);
                if (r.containsPoint (*world_pos))
                {
                    _last_ite = WallComponents_start;
                }
            }

            {
                CCRect r = CCRectMake (_end.x*_width, _end.y*_width, _width, _width);
                if (r.containsPoint (*world_pos))
                {
                    _last_ite = WallComponents_end;
                }
            }


            if (_last_ite == WallComponents_none)
                setWall (ccp (x,y));



            _bChanged = true;
            _cv.notify_all();



        }
        break;
        case    Telegram_TOUCH_CANCEL:
        case  Telegram_TOUCH_END:
        {
            _last_ite = WallComponents_none;
        }
        break;
        case    Telegram_DRAW:
        {
            {

                int i = 0;

                for (auto a : _shorest)
                {
                    if (i == 0)
                    {
                        ++i;
                        continue;;
                    }
                    if (i == _shorest.size())break;

                    ccColor4F c = { 1, 1, 1, 1 };

                    ccDrawSolidRect (ccp (a.x*_width, a.y*_width), ccp ((a.x + 1)*_width, (a.y + 1)*_width), c);
                    ++i;
                    if (i>=_shorest.size())break;
                }

            }

            {
                static float middle = 0.3f;
                for (auto & p : _nodes)
                    ccDrawSolidRect (p*_width, ccpAdd (p, { 1, 1 })*_width, { 1, middle, middle, 1

                                                                            });
                ccDrawSolidRect (ccp (_start.x*_width, _start.y*_width), ccp ((_start.x + 1)*_width, (_start.y + 1)*_width), { 1, 1, middle, 1 });
                ccDrawSolidRect (ccp (_end.x*_width, _end.y*_width), ccp ((_end.x + 1)*_width, (_end.y + 1)*_width), { middle, middle, 1, 1 });




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



};

//
//#include "CCHexagon/Hexagon.h"
//#include "CCHexagon/HexCoordinate.h"
//class HexWallComponents :public Component
//{
//
//    std::mutex					_cv_m;
//    std::condition_variable		_cv;
//    algorithm::Graph<Hexagon>	_graph;
//    CCPoint						_ccp_RL;
//    set<Hexagon>			_nodes;
//    list<Hexagon>				_shorest;
//    int							_width;
//    HexCoordinate				_coordinate;
//    std::thread					_thread;
//    CCLabelTTF*					_access_count;
//    Hexagon 					_start;
//    Hexagon 					_end;
//
//    struct ccp_ext {
//        Hexagon  p;
//        int      iAccessTimes;
//        Hexagon  direction;
//    };
//    vector<ccp_ext>			    _access_nodes;
//
//public:
//
//
//    void setWall(Hexagon& p)
//    {
//        p.Integerlize();
//        _graph.SetNodeValidate (p, false);
//        _coordinate.RemoteHex(p);
//        _nodes.insert (p);
//    }
//    static  HexWallComponents* Create(int width)
//    {
//        return new HexWallComponents(width);
//    }
//    ~HexWallComponents()
//    {
//
//    }
//
//    HexWallComponents(int w) :Component(Component_HexWALLS), _width(w), _thread(std::bind(&HexWallComponents::construct, this))
//        , _coordinate(w)
//    {
//
//
//        _access_count = CCLabelTTF::create ("", "Helvetica", 16);
//        _access_count->retain();
//        _start = HexZero;
//        _end = HexZero;
//        MakeGraph();
//
//    }
//
//    void  MakeGraph ( )
//    {
//        _graph.clear();
//        _coordinate.MakeRect2(9, 7, Hexagon(0, 0), ccc4f(1, 0, 0.5, 1));
//        HexCoordinate      _intersection;
//        _intersection.setMask(&_coordinate.hexagones);
//
//        for (auto& a : _coordinate.hexagones) {
//            _intersection.hexagones.clear();
//            _intersection.MakeRingHex(1, a);
//
//            for (auto b : _intersection.hexagones) {
//                _graph._OL->addNEdge(a, b, 1);
//            }
//        }
//
//    }
//
//    void construct()
//    {
//
//        while (true) {
//            std::unique_lock<std::mutex> lk (_cv_m);
//            Hexagon oldend = _end;
//
//            _cv.wait (lk, [=] {return  oldend!=_end; });
//
//
//
//            _shorest.clear();
//
//            auto from = _graph.findNode (_start);
//            auto  current = _graph.findNode (_end);
//
//
//            {
//                _graph.Dijkstra(from, current/*, [](const CCPoint& a, const CCPoint& b) {
//                    return  a.getDistance (b)*1.2;
//                }    */ );
//            }
//
//
//
//            while (current && current != from) {
//                _shorest.push_back (current->data);
//                current = current->p;
//            }
//
//            _cv.notify_all();
//        }
//
//    }
//
//    bool HandleMessage (const Telegram& msg)
//    {
//        switch (msg.Msg) {
//
//        case    Telegram_UPDATE: {
//
//        }
//        break;
//        case Telegram_ACCESS_NODE: {
//
//            Hexagon * world_pos = reinterpret_cast<Hexagon*> ((int)(msg.args[0]));
//            //  CCPoint * world_parent_pos = reinterpret_cast<CCPoint*> ((int) (msg.args[1]));
//            Hexagon  direction = *world_pos;// ccpSub(*world_parent_pos, *world_pos).normalize();
//            auto  ite = find_if (_access_nodes.begin(), _access_nodes.end(), [=] (const  ccp_ext & ce) {
//                return ce.p == *world_pos;
//            });
//            if (ite != _access_nodes.end()) {
//                ite->iAccessTimes++;
//                ite->direction=direction;
//            } else {
//                _access_nodes.push_back ({ *world_pos, 1 });
//                _access_nodes.rbegin()->direction = direction;
//            }
//        }
//        break;
//
//        case Telegram_CLIK_ON_WALL: {
//
//            CCPoint  world_pos = ccp(msg.args[0], msg.args[1]);
//            Hexagon  world_hex = _coordinate.CCP2Hex(world_pos);
//            int x = world_pos.x / _width;
//            int y = world_pos.y / _width;
//
//            // if (_nodes.size() < 20) {
//            setWall(world_hex);
//            // } else
//            //      _end = world_hex;
//
//            _cv.notify_all();
//
//
//
//        }
//        break;
//        case    Telegram_DRAW: {
//
//            int i = 0;
//
//            for (auto a : _shorest) {
//                if (i == 0) {
//                    ++i;
//                    continue;;
//                }
//                if (i == _shorest.size())break;
//
//                ccColor4F c = { 1, 1, 1, 1 };
//                a.Draw(&_coordinate);
//                ++i;
//                if (i>=_shorest.size())break;
//            }
//
//            _coordinate.draw();
//
//
//
//        }
//        break;
//
//        default:
//            break;
//        }
//        return false;
//
//    }
//
//
//
//};
class BezierCompont :public Component
{
public:

    enum  eConst
    {
        none=-1,
        from1,
        to1,
        from2,
        to2,
        count,
    };
    struct bezierNode
    {
        CCPoint		node;
        CCPoint		handle;
    };
    vector<bezierNode> p;
    /*CCPoint p[count];*/

    CCDrawNode* drawNode;
    float   radius;
    CCPoint* selected_node;

    static  BezierCompont* Create( )
    {
        return new BezierCompont( );
    }
    ~BezierCompont()
    {
        drawNode->release();
    }
    BezierCompont( ) :Component (Component_Bezier)
    {
        p.resize (2);
        p[0].node = ccp (100,100);
        p[0].handle = ccp (100,300 );

        p[1].node = ccp (300, 100);
        p[1].handle = ccp (300, 300);
        selected_node=NULL;
        radius=15;
        drawNode=CCDrawNode::create();
        drawNode->retain();
    }
    bool inCircile (CCPoint& center, float radius,CCPoint& click)
    {
        return ccpSub (center,click).getLength()<=radius;
    }
    void drawSolidCircle (CCPoint& center, float radius, ccColor4B  color )
    {
        ccDrawColor4B (color.r, color.g, color.b, color.a);
        for (int i = 0; i < radius; i++)
        {
            ccDrawCircle (center, i, 0, 50, false);
        }
    }
    void drawBezier (bezierNode& l1,bezierNode &l2)
    {

        static int persision = 100;


        //draw bezier
        CCPoint l1_seg = (l1.handle-l1.node) / persision;
        CCPoint l2_seg = (l2.node - l2.handle) / persision;

        ccDrawColor4B (255, 122, 122, 255);
        CCPoint from = l1.node;
        for (int i = 0; i <= persision; ++i)
        {
            CCPoint f = l1.node + l1_seg*i;
            CCPoint t = l2.handle + l2_seg*i;
            ccDrawLine (from, (t - f) / persision*i + f);
            from = (t - f) / persision*i + f;
        }

    }
    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {


        case Telegram_TOUCH_BEGIN:
        {
            CCPoint * world_pos = reinterpret_cast<CCPoint*> ((int) (msg.args[0]));
            selected_node=NULL;
            int i=none+1;
            for (auto& a: p)
            {
                if (inCircile (a.node, radius, *world_pos))
                {
                    selected_node=&a.node;
                    return true;
                }
                if (inCircile (  a.handle, radius, *world_pos))
                {
                    selected_node = &a.handle;
                    return true;
                }
                ++i;
            }
            p.push_back ({ *world_pos, *world_pos  });
            selected_node= & (p.rbegin()->handle);
            return true;
        }
        break;
        case Telegram_TOUCH_MOVE:
        {
            CCPoint * world_pos = reinterpret_cast<CCPoint*> ((int) (msg.args[0]));
            if (selected_node != NULL)
            {
                *selected_node = *world_pos;
            }
        }
        break;
        case Telegram_TOUCH_CANCEL:
        case Telegram_TOUCH_END:
        {
            selected_node = NULL;
        }
        break;
        case    Telegram_DRAW:
        {

            drawNode->clear();

            for (auto& a:p)
            {

                drawSolidCircle (a.node, radius, { 122, 255, 122, 255 });
                drawSolidCircle (a.handle, radius, { 122, 255, 255, 255 });
            }


            for (auto& a : p)
            {
                ccDrawColor4B (255, 255, 255, 255);

                ccDrawLine (a.node,  a.handle);

            }



            for (int i=0; i<p.size()-1; ++i)
            {
                ccDrawColor4B (255, 255, 255, 255);
                drawBezier (p[i],p[i+1]);
            }


            drawNode->visit();


        }
        break;

        default:
            break;
        }
        return false;

    }
};

class SmoothLineCompont :public Component
{
public:

    enum  eConst
    {
        none = -1,
        from1,
        to1,
        from2,
        to2,
        count,
    };

    vector<CCPoint> p;
    /*CCPoint p[count];*/

    CCDrawNode* drawNode;
    float   radius;
    CCPoint* selected_node;

    static  SmoothLineCompont* Create()
    {
        return new SmoothLineCompont();
    }
    ~SmoothLineCompont()
    {
        drawNode->release();
    }
    SmoothLineCompont() :Component (Component_SmoothLine)
    {
        p.resize (2);
        p[0]  = ccp (100, 100);

        selected_node = NULL;
        radius = 15;
        drawNode = CCDrawNode::create();
        drawNode->retain();
    }
    bool inCircile (CCPoint& center, float radius, CCPoint& click)
    {
        return ccpSub (center, click).getLength() <= radius;
    }
    void drawSolidCircle (CCPoint& center, float radius, ccColor4B  color)
    {
        ccDrawColor4B (color.r, color.g, color.b, color.a);
        for (int i = 0; i < radius; i++)
        {
            ccDrawCircle (center, i, 0, 50, false);
        }
    }
    void drawBezier (CCPoint& n1, CCPoint& h1, CCPoint&n2, CCPoint&h2)
    {

        static int persision = 100;


        //draw bezier
        CCPoint l1_seg = (h1 - n1) / persision;
        CCPoint l2_seg = (n2 - h2) / persision;

        ccDrawColor4B (255, 122, 122, 255);
        CCPoint from = n1;
        for (int i = 0; i <= persision; ++i)
        {
            CCPoint f = n1 + l1_seg*i;
            CCPoint t = h2+ l2_seg*i;
            ccDrawLine (from, (t - f) / persision*i + f);
            from = (t - f) / persision*i + f;
        }

    }
    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {


        case Telegram_TOUCH_BEGIN:
        {
            CCPoint * world_pos = reinterpret_cast<CCPoint*> ((int) (msg.args[0]));
            selected_node = NULL;

            for (auto& a : p)
            {
                if (inCircile (a, radius, *world_pos))
                {
                    selected_node = &a;
                    return true;
                }

            }
            p.push_back ({ *world_pos });
            selected_node = & (*p.rbegin() );
            return true;
        }
        break;
        case Telegram_TOUCH_MOVE:
        {
            CCPoint * world_pos = reinterpret_cast<CCPoint*> ((int) (msg.args[0]));
            if (selected_node != NULL)
            {
                *selected_node = *world_pos;
            }
        }
        break;
        case Telegram_TOUCH_CANCEL:
        case Telegram_TOUCH_END:
        {
            selected_node = NULL;
        }
        break;
        case    Telegram_DRAW:
        {

            drawNode->clear();

            for (auto& a : p)
            {

                drawSolidCircle (a, radius, { 122, 255, 122, 255 });

            }



            for (int i = 0; i < p.size() - 2; ++i)
            {
                ccDrawColor4B (255, 255, 255, 255);
                drawBezier (p[i], p[i+1] ,p[i + 1],p[i+2]);
            }


            drawNode->visit();


        }
        break;

        default:
            break;
        }
        return false;

    }
};

class RayTraceComponents :public Component
{
    enum eConst
    {
        OK,
        OVERLAP,
        PARALLEL,
        NOT_INTERSECTION,
    };

    struct Segment
    {
        CCPoint segStart;
        CCPoint segEnd;
    };

    struct Ray
    {
        CCPoint rayStart;
        CCPoint rayDir;

        //ax+by+c=0;
        void SolveLine (Segment&s, float&a, float& b, float& c)
        {
            if (s.segStart.x == s.segEnd.x && s.segStart.y != s.segEnd.y)   //parallel with y
            {
                a = 1;
                b = 0;
                c = -s.segStart.x;
            }
            else if (s.segStart.x != s.segEnd.x && s.segStart.y == s.segEnd.y)   //parallel with x
            {
                a = 0;
                b = 1;
                c = -s.segStart.y;
            }
            else
            {
                a = - (s.segStart.y - s.segEnd.y) / (s.segStart.x - s.segEnd.x);
                b = 1;
                c = - (s.segStart.y*s.segEnd.x - s.segEnd.y*s.segStart.x) / (s.segEnd.x - s.segStart.x);
            }
        }

        eConst intersect (Segment& w, CCPoint& intersection,float & T)
        {

            float A,B,C;
            SolveLine (w,A,B,C);

            if (A*rayDir.x + B*rayDir.y == 0)return NOT_INTERSECTION;
            T = (-A*rayStart.x - C - B*rayStart.y) / (A*rayDir.x + B*rayDir.y);
            if (T<0)return NOT_INTERSECTION;

            float ix = rayStart.x + rayDir.x*T;
            float iy = rayStart.y + rayDir.y*T;
            float yy = A*ix+B*iy+C;
            intersection = ccp (ix,iy);
            CCPoint d1 = intersection - w.segStart;
            CCPoint d2 = intersection - w.segEnd;
            //in segment
            float f= (d1+d2).getLength();
            if (f<d1.getLength() || f<d2.getLength())
                return OK;


        }

    };



    CCPoint				_light;
    bool				b_move_light;
    vector<Segment>    walls;
    vector<CCPoint> edge_nodes;


public:
    void reOrderScanNodes()
    {
        std::sort (edge_nodes.begin(), edge_nodes.end(), [=] (CCPoint& a,CCPoint& b)
        {
            return ccpAngle (_light, b)<ccpAngle (_light, a);
        });
    }
    void reConstructScanNodes()
    {
        edge_nodes.clear();
        for (auto &a : walls)
        {
            edge_nodes.push_back (a.segStart);
            edge_nodes.push_back (a.segEnd);
        }
    }
    static  RayTraceComponents* Create( )
    {
        return new RayTraceComponents( );
    }
    ~RayTraceComponents() {}
    RayTraceComponents () :Component (Component_Raytrace)
    {
        CCSize s=CCDirector::sharedDirector()->getWinSize();
        walls=
        {
            {
                { 100, 100 }, { 400, 100 }
            },
            {
                { 50, 50 }, { 50, s.height-50 }
            },
            {
                { 50, 50 }, { s.width-50, 50 }
            },
            {
                { 50, s.height-50 }, { s.width-50, s.height-50 }
            },
            {
                { s.width-50, 50 }, { s.width-50, s.height-50 }
            },
            {
                { 700, 200 }, { 300, 500 }
            }
        };
        _light = ccp (200,200);
        b_move_light=false;
        reConstructScanNodes();
        reOrderScanNodes();


    }




    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {

        case    Telegram_UPDATE:
        {

        }
        break;
        case Telegram_ACCESS_NODE:
        {


        }
        break;
        case  Telegram_TOUCH_DOUBLE:
        {

        }
        break;
        case Telegram_TOUCH_MOVE:
        {
            if (b_move_light)
            {
                CCPoint * world_pos = reinterpret_cast<CCPoint*> ((int) (msg.args[0]));
                _light = *world_pos;
            }
        }
        break;
        case Telegram_TOUCH_BEGIN:
        {
            b_move_light=true;
        }
        break;
        case    Telegram_TOUCH_CANCEL:
        case  Telegram_TOUCH_END:
        {
            b_move_light=false;
        }
        break;
        case    Telegram_DRAW:
        {
            ccDrawColor4B (255, 0, 0, 255);
            for (auto& a : walls)
            {
                ccDrawLine (a.segStart,a.segEnd);
            }
            ccPointSize (10);
            ccDrawPoint (_light);


            // TEMPLATE STRUCT less

            struct less
                    : public binary_function<CCPoint, CCPoint, bool>
            {
                // functor for operator<
                bool operator() (const CCPoint& _Left, const CCPoint& _Right) const
                {

                    return (_Left.x < _Right.x || (_Left.x == _Right.x && _Left.y < _Right.y));
                }
            };

            ccDrawColor4B (255, 255, 255, 255);
            map<CCPoint, vector<CCPoint>, less>   lightWallSegments;

            for (auto & edge_node: edge_nodes)
            {
                Ray beam = {_light,edge_node-_light};
                lightWallSegments[edge_node];
                for (auto & wall: walls)
                {

                    CCPoint intersection;
                    float T;
                    auto ret = beam.intersect (wall, intersection, T);

                    if (ret == OK )
                    {
                        if (lightWallSegments[edge_node].size()==0)
                            lightWallSegments[edge_node].push_back (intersection);
                        else
                        {
                            auto a = lightWallSegments[edge_node][0];
                            if ((a - _light).getLength() > (intersection - _light).getLength())
                            {
                                lightWallSegments[edge_node].clear();
                                lightWallSegments[edge_node].push_back (intersection);
                            }

                        }
                    }

                }

            }

            for (auto & l: lightWallSegments)
            {
                CCPoint p;
                if (l.second.size()>0)
                    p=l.second[0] ;
                else
                    p= l.first ;

                ccDrawLine (_light, p);
                ccPointSize (5);
                ccDrawPoint (p);
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
    return false;
    MoveComponent* m = dynamic_cast<MoveComponent*> (c->GetC (Component_MOVE));
    //MoveComponent* m2 = dynamic_cast<MoveComponent*> (c->GetC(Component_MOVE));
    auto  _winsize = CCDirector::sharedDirector()->getWinSize();
    if (_winsize.width < (m->_pos.x) || _winsize.height < (m->_pos.y) || (m->_pos.x) < 0 || (m->_pos.y < 0))
    {
        return true;
    }
    return false;
}
