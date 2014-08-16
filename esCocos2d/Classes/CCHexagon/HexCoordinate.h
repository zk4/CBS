#pragma  once
//  Created by ZK on 2014/5/23.

#include "Hexagon.h"
#include "cocos2d.h"
#include <vector>
using namespace std;

class HexCoordinate
{
public:
    //cocos2d::CCPoint zeroPoint;
    float length;
    vector<Hexagon>   hexagones;
    vector<Hexagon>*  mask;
    bool			  _bBasedonMask;
    CCAffineTransform matrix;

    HexCoordinate (float length_ = 10,   CCAffineTransform  m_ = { sqrt (3.0f), 0, sqrt (3.0f) / 2.0f, -1.5f,0,0 },vector<Hexagon>* mask=NULL);

    void				setMask (vector<Hexagon>* mask, bool bBasedonMask_=true);
    void				setMatrix (CCAffineTransform& m);
    void				setZeroPoint (int x,int y);
    void				draw	      ();
    cocos2d::CCPoint	Hex2CCP       (const Hexagon&  h) const;
    Hexagon				CCP2Hex       (const cocos2d::CCPoint&  p)const;
    bool                Find          (const Hexagon& target)const;
    bool				RemoteHex	  (const Hexagon& target);

    void		CopyCoordinate (HexCoordinate& c);
    //make factory
    void                MakeRect2     (int width, int height, const Hexagon& lb, ccColor4F color = ccc4f (1, 1, 1, 1));
    void                MakeRect      (int width, int height, const Hexagon& lb, ccColor4F color  = ccc4f (1, 1, 1, 1) );
    void                MakeLine      (const Hexagon& start, const Hexagon& end, ccColor4F color = ccc4f (1, 1, 1, 1));
    void                InterSect     (const vector<Hexagon>& src);
    void                MakeCross	  (int radius, const Hexagon& center, ccColor4F color = ccc4f (1, 1, 1, 1));
    void                MakeRingHexes (int inner, int outter, const Hexagon& center, ccColor4F color = ccc4f (1, 1, 1, 1));
    void                MakeRingHex   (int radius, const Hexagon& center, ccColor4F color = ccc4f (1, 1, 1, 1));
    void                MakeSolidHex  (int radius, const Hexagon& center, ccColor4F color = ccc4f (1, 1, 1, 1));

private:
    void				doMask();
};


