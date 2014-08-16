#include "HexCoordinate.h"
#include "BeeHive.h"
#include <algorithm>
#include <math.h>
using namespace std;

void HexCoordinate::draw()
{
    for (auto& hexagon : hexagones) {
        hexagon.Draw (this );
    }
}


HexCoordinate::HexCoordinate (float length_, CCAffineTransform  m_, vector<Hexagon>* mask_ ) :
    length (length_),
    matrix (m_),
    mask (mask_)
{
}

cocos2d::CCPoint HexCoordinate::Hex2CCP (const Hexagon&  h) const
{
    return  CCPointApplyAffineTransform (ccp (h.q*length, h.r*length), matrix);
}

Hexagon HexCoordinate::CCP2Hex (const CCPoint&  p) const
{
    CCPoint pp = CCPointApplyAffineTransform (p, CCAffineTransformInvert (matrix));
    return Hexagon (pp.x / length, pp.y / length)  ;
}



void HexCoordinate::MakeSolidHex (int radius, const Hexagon& center, ccColor4F color)
{
    BeeHive::MakeSolidHex (radius, center, hexagones, color);
    doMask();
}

void HexCoordinate::MakeRingHex (int radius, const Hexagon& center, ccColor4F color)
{
    BeeHive::MakeRingHex (radius, center, hexagones, color);
    doMask();
}

void HexCoordinate::MakeRingHexes (int inner, int outter, const Hexagon& center, ccColor4F color)
{
    BeeHive::MakeRingHexes (inner, outter, center, hexagones, color);
    doMask();
}



void HexCoordinate::MakeCross (int radius, const Hexagon& center, ccColor4F color)
{
    BeeHive::MakeCross (radius, center, hexagones, color);
    doMask();
}

void HexCoordinate::InterSect (const vector<Hexagon>& src)
{
    BeeHive::InterSect (hexagones, src);
    doMask();
}

bool HexCoordinate::Find (const Hexagon& target) const
{
    return   BeeHive::Find (hexagones, target);
}



void HexCoordinate::MakeLine (const Hexagon& start, const Hexagon& end, ccColor4F color /*= ccc4f(1, 1, 1, 1)*/)
{
    BeeHive::MakeLine (start, end, hexagones, color);
    doMask();
}

void HexCoordinate::MakeRect (int width, int height, const Hexagon& lb, ccColor4F color /*= ccc4f (1, 1, 1, 1)*/)
{
    BeeHive::MakeRect (width, height, lb, hexagones, color);
    doMask();
}

void HexCoordinate::setMatrix (CCAffineTransform& m)
{
    matrix=m;
}


void HexCoordinate::CopyCoordinate (HexCoordinate& c)
{
    length=c.length;
    matrix=c.matrix;
}

void HexCoordinate::MakeRect2 (int width, int height, const Hexagon& lb, ccColor4F color /*= ccc4f (1, 1, 1, 1)*/)
{
    BeeHive::MakeRect2 (width, height, lb, hexagones, color);
    doMask();
}

void HexCoordinate::setZeroPoint (int x, int y)
{
    matrix.tx=x;
    matrix.ty=y;
}

void HexCoordinate::setMask (vector<Hexagon>*mask_, bool bBasedonMask_)
{
    mask=mask_;
    _bBasedonMask=bBasedonMask_;
}

void HexCoordinate::doMask()
{
    if (mask) {
        for (int i=hexagones.size()-1; i>=0; --i) {
            bool found =false;
            for (auto & a : *mask) {
                if (a == hexagones[i]) {
                    if (!_bBasedonMask)
                        hexagones.erase (hexagones.begin() + i);
                    else
                        found = true;
                    break;
                }
            }
            if (_bBasedonMask && !found)
                hexagones.erase (hexagones.begin() + i);
        }
    }
}

bool HexCoordinate::RemoteHex(const Hexagon& target)
{

    remove(hexagones.begin(),hexagones.end(),target);
    return true;
}




