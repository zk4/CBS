#include "MagicComponent.h"
#include "es/ComponentMgr.h"
#include "Configuration.h"
#include "messaging/MessageDispatcher.h"
#include "components/Components.h"
#include "particle_nodes/CCParticleExamples.h"
#include "math/zkMath.h"
#include "Perlin_Noise/PerlinNoise.h"
#include "Tools.h"
// Define the size of the image
static unsigned int width = 960, height = 640;
using namespace cocos2d;
static const char s_fire[] = "fire.png";
MagicComponent::MagicComponent ( CCRenderTexture*  rt ) : Component ( Component_MAGIC ), _rt ( rt ), image ( width, height ) {
    _emitter = NULL;
    _b_pressed = false;
    _pBrush = CCSprite::create ( "stroke.png" );
    _pBrush->retain();

    _drawNode = CCDrawNode::create();
    _drawNode->retain();
    _drawNode->clear();




    // Save the image in a binary PPM file
//   image.write ( "figure_8_R.ppm" );


}
static void  generatePoint ( CCPoint& p, vector<CCPoint>& outer ) {

    outer.push_back ( p );
    auto s = CCDirector::sharedDirector()->getWinSize();
    zkMath::Line l = { 1, 0, -s.width / 2 };
    ;
    outer.push_back ( zkMath::symmetricalPointofLine ( p, l ) );
}
bool MagicComponent::HandleMessage ( const Telegram& msg ) {
    switch ( msg.Msg ) {
    case Telegram_UPDATE: {
        float f = msg.args[0];
        /* if ( _emitter ) {
             _emitter->update ( f );
             if ( _b_pressed ) {
                 _rt->begin();
                 auto oldP = _emitter->getPosition();
                 static vector<CCPoint>  ps;
                 ps.clear();
                 generatePoint ( oldP, ps );
                 for ( auto & a : ps ) {
                     _emitter->setPosition ( a );
                     _emitter->visit();
                 }
                 _emitter->setPosition ( oldP );
                 _rt->end();
             }
         }*/



    }
    break;
    case Telegram_TOUCH_BEGIN: {
        CCPoint * world_pos = reinterpret_cast<CCPoint*> ( ( int ) ( msg.args[0] ) );
        _b_pressed = true;
        /*   _emitter = CCParticleGalaxy::create();
           _emitter->retain();
           _emitter->setTexture ( CCTextureCache::sharedTextureCache()->addImage ( s_fire ) );
           _emitter->setPosition ( *world_pos );
        */
        // Create a PerlinNoise object with the reference permutation vector
        PerlinNoise pn ( Tools::RandomBetween ( 1, 10000 ) );

        unsigned int kk = 0;
        // Visit every pixel of the image and assign a color generated with Perlin noise

        for ( unsigned int i = 0; i < height; ++i ) { // y
            for ( unsigned int j = 0; j < width; ++j ) { // x
                double x = ( double ) j / ( ( double ) width );
                double y = ( double ) i / ( ( double ) height );

                // Typical Perlin noise
                double n = pn.noise ( 40 * x,   50 * y, 100 );

                // Wood like structure
                /*  n = 20 * pn.noise ( x, y, 0.8 );
                  n = n - floor ( n );*/

                // Map the values to the [0, 255] interval, for simplicity we use
                // tones of grey
                /*   image.r[kk] = floor ( 255 * n );
                   image.g[kk] = floor ( 255 * n );
                   image.b[kk] = floor ( 255 * n );*/

                _drawNode->drawDot ( ccp ( i, j ), 1, { n, n, n, 1 } );
                kk++;
            }
        }

        _rt->clear ( 0, 0, 0, 0 );
        _rt->begin();
        _drawNode->visit();
        _rt->end();
        _drawNode->clear();
    }
    break;
    case	Telegram_TOUCH_MOVE: {
        if ( _b_pressed ) {
            //CCPoint * world_pos = reinterpret_cast<CCPoint*> ( ( int ) ( msg.args[0] ) );
            ///*   _emitter->setPosition ( *world_pos );*/

            //_rt->begin();

            //_pBrush->setPosition ( *world_pos );
            //// _pBrush->setRotation ( rand() % 360 );
            //float r = ( float ) ( rand() % 3 / 20.f );
            ////  _pBrush->setScale ( r );
            //_pBrush->setColor ( ccc3 ( rand() % 127 + 128, 255, 0 ) );

            ////set anchor point at screen center
            //auto winSize = CCDirector::sharedDirector()->getWinSize();
            //auto p = _pBrush->convertToNodeSpace ( winSize / 2 );
            //auto pSize = _pBrush->getContentSize();
            //auto anchor = ccp ( p.x / pSize.width, p.y / pSize.height );
            //_pBrush->setAnchorPoint ( anchor );


            //_pBrush->setRotation ( 3.14 / 2 );
            //_pBrush->visit();
            //_pBrush->setRotation ( 0 );
            //_rt->end();
        }


    }
    break;
    case	Telegram_TOUCH_END:
    case	Telegram_TOUCH_CANCEL: {
        _b_pressed = false;
        if ( _emitter ) {
            _emitter->release();
            _emitter = NULL;
        }
    }
    break;
    case Telegram_VISIT: {

        _rt->visit();
    }
    break;

    default:
        break;
    }
    return false;
}

MagicComponent::~MagicComponent() {
    _pBrush->release();
    _drawNode->release();
}
