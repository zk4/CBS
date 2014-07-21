#pragma  once

#include "es/Component.h"
#include "Configuration.h"
#include "messaging/MessageDispatcher.h"
#include "cocos2d.h"
#include "es/ComponentMgr.h"
using namespace cocos2d;
static bool ifOutWindow (Component* c);
  


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
	void addChild(CCNode* c)
	{
		_delegate->addChild(c);
	
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


        case Telegram_ROTATE:
        {

            _delegate->setRotation (_delegate->getRotation() + msg.args[0]);
        }
        break;
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
        double			 dFriction¦Ì
    )
    {
        return new MoveComponent (
                   vPos,
                   dMass,
                   dMaxSpeed,
                   dMaxForce,
                   dMaxTurnRate,
                   dFriction¦Ì
               );
    }


    void update (CCPoint  SteeringForce,float time_elapsed)
    {


        //Acceleration = Force/Mass
        CCPoint friction = (ccpLength (_vVelocity)>0) ? -_vVelocity.normalize()*_dFrictionMu*_dMass: CCPointZero;


        SteeringForce = friction + SteeringForce;

        auto  acceleration =  SteeringForce   / _dMass;


        //update velocity

        _vVelocity = _vVelocity+ acceleration * time_elapsed;

        //make sure vehicle does not exceed maximum velocity
        if (ccpLength (_vVelocity) > _dMaxSpeed)
        {
            _vVelocity = _vVelocity.normalize()*_dMaxSpeed;
        }

        //update the position
        _pos = _pos+ _vVelocity * time_elapsed;

        CCSize size = CCDirector::sharedDirector()->getWinSize();
        if (_pos.x<0 || _pos.x>size.width)
            _vVelocity.x = -_vVelocity.x;

        if (_pos.y<0 || _pos.y>size.height)
            _vVelocity.y = -_vVelocity.y;



        //update the heading if the vehicle has a non zero velocity
        if (ccpLength (_vVelocity) > 0.00000001)
        {
            //  _vHeading =   _vVelocity.normalize() ;

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


            CenterOfMass  = CenterOfMass+ neighbors[a]->_pos;

            ++NeighborCount;

        }

        if (NeighborCount > 0)
        {
            //the center of mass is the average of the sum of positions
            CenterOfMass  = CenterOfMass/ (double)NeighborCount;

            //now seek towards that position
            SteeringForce = Seek (CenterOfMass);
        }

        //the magnitude of cohesion is usually much larger than separation or
        //allignment so it usually helps to normalize it.
        return    SteeringForce.normalize() ;
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

       // assert (0);
		return target ;
        //project the target into world space
        /*CCPoint Target = PointToWorldSpace(target,
        	_vHeading,
        	_vSide,
        	_pos);
        */
        //and steer towards it
        //  return Target - _pos;
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
            CCPoint HidingSpot = GetHidingPosition ((*curOb)->_pos ,
                                                    (*curOb)->_dBoundingRadius ,
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
        CCPoint ToOb =   (posOb - posHunter).normalize();

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
                AverageHeading  = AverageHeading + neighbors[a]->_vHeading;

                ++NeighborCount;
            }
        }

        //if the neighborhood contained one or more vehicles, average their
        //heading vectors.
        if (NeighborCount > 0)
        {
            AverageHeading = AverageHeading/ (double)NeighborCount;

            AverageHeading = AverageHeading- _vHeading;
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
        ttf_head->setColor (ccc3 (255, 0, 255 ));

    }
    CCLabelTTF* ttf_velocity;
    CCLabelTTF* ttf_head;
    bool HandleMessage (const Telegram& msg)
    {
        switch (msg.Msg)
        {
        case Telegram_DRAW:
        {
            if (ccpLength (_vVelocity)>0.5)
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
            update (_vSteeringForce, msg.args[0]);
            DD (GetParent()->GetID(), Telegram_SET_POS, { _pos.x, _pos.y });
        }
        break;
        case Telegram_ARRIVE:
        {
            DD (GetParent()->GetID(), Telegram_SEARCH, {});
            _vSteeringForce =  Arrive (ccp (msg.args[0], msg.args[1]), fast);
            _vHeading = ccp (msg.args[0], msg.args[1])-_pos;
            update (_vSteeringForce,  0.1);


        }
        break;
        case Telegram_AI:
        {
            DD (GetParent()->GetID(), Telegram_SEARCH, {});
			static int count =0;
			if (count<1000)
			{_vSteeringForce = Separation();
			 
			}
			count++;
			if (count>1000)
			{
				if (ids_insight.size()>0)
				_vSteeringForce = Pursuit((MoveComponent*)CompMgr->GetComponentFromID(ids_insight[rand() % (ids_insight.size())]));
			}



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
                HP=0;
                DD (  Telegram_DEAD, { (double) GetParent()->GetID() });
            }
        }
        break;
        case Telegram_DRAW:
        {
            auto moveC = (MoveComponent*)GetParent()->GetC (Component_MOVE);
            ccDrawSolidRect (moveC->_pos+ ccp (-HP/2,40), moveC->_pos + ccp (HP/2, 45),  {255,0,0,122});
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
                    Component* a=NULL;
                    for (auto c : ids_insight)
                    {

                        auto hpC = (HPComponent*)CompMgr->GetComponentFromID (c)->GetC (Component_HP);
                        if (hpC && hpC->HP>0)
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
        case     Telegram_DRAW  :
        {
            auto moveC = (MoveComponent*)GetParent()->GetC (Component_MOVE);

            if (moveC)
            {
                ccDrawColor4B (122, 122, 255, 255);
                ccDrawLine (moveC->_pos, _ccpTarget);
            }
        }
        break;
        case  Telegram_SEARCH_RESULT:
        {
            if ( ids_insight.empty())
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
        Dispatcher->DispatchMessageZZ (0, GetID(), id, Telegram_HURT, { hp });
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
	Box2DComponent() :Component(Component_BOX2D)
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		// Create sprite and add it to the layer
		_ball = CCSprite::create("1.png" );
		_ball->setPosition(winSize/2);
		_ball->retain();

		// Create a world
		b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
		bool doSleep = true;
		_world = new b2World(gravity );

		// Create edges around the entire screen
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0, 0);
		b2Body *groundBody = _world->CreateBody(&groundBodyDef);
		b2PolygonShape groundBox;
		b2FixtureDef boxShapeDef;
		boxShapeDef.shape = &groundBox;
		 
		groundBox.SetAsBox(4 , winSize.height / PTM_RATIO/2 );
		groundBody->CreateFixture(&boxShapeDef);
		 

		// Create ball body and shape
		b2BodyDef ballBodyDef;
		ballBodyDef.type = b2_dynamicBody;
		ballBodyDef.position.Set(winSize.width / 2 / PTM_RATIO, winSize.height / 2 / PTM_RATIO);
		ballBodyDef.userData = _ball;
		_body = _world->CreateBody(&ballBodyDef);

		b2CircleShape circle;
		circle.m_radius = 26.0 / PTM_RATIO;

		b2FixtureDef ballShapeDef;
		ballShapeDef.shape = &circle;
		ballShapeDef.density = 1.0f;
		ballShapeDef.friction = 0.2f;
		ballShapeDef.restitution = 0.8f;
		_body->CreateFixture(&ballShapeDef);

		 
	}
	void tick(float dt){

		_world->Step(dt, 10, 10);
		for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext()) {
			if (b->GetUserData() != NULL) {
				CCSprite *ballData = (CCSprite *)b->GetUserData();
				ballData->setPosition (ccp(b->GetPosition().x * PTM_RATIO,
					b->GetPosition().y * PTM_RATIO));
				ballData->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
			}
		}

	}
	bool HandleMessage(const Telegram& msg)
	{
		switch (msg.Msg)
		{
	 
		case    Telegram_UPDATE:
		{
				tick(msg.args[0]);
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
