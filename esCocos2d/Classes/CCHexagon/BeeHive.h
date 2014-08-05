#pragma  once
//  Created by ZK on 2014/5/23.


#include "cocos2d.h"
#include "HexCoordinate.h"
using namespace cocos2d;
struct BeeHive
{

    static void   MakeSolidHex (int radius, const Hexagon& center, vector<Hexagon>& hexagons, ccColor4F color = ccc4f (1, 1, 1, 1));
    static void   MakeRingHex (int radius, const Hexagon& center, vector<Hexagon>& ring, ccColor4F color = ccc4f (1, 1, 1, 1));
    static void   MakeRingHexes (int inner, int outter, const Hexagon& center, vector<Hexagon>& rings, ccColor4F color = ccc4f (1, 1, 1, 1));
    static void   MakeCross (int radius, const Hexagon& center, vector<Hexagon>& cross, ccColor4F color = ccc4f (1, 1, 1, 1));
    static void   MakeRect (int width, int height, const Hexagon& lb, vector<Hexagon>& cross, ccColor4F color = ccc4f (1, 1, 1, 1));
    static void   MakeLine (const Hexagon& start, const Hexagon& end, vector<Hexagon>& line, ccColor4F color = ccc4f (1, 1, 1, 1));
    static void   MakeRect2 (int width, int height, const Hexagon& lb, vector<Hexagon>& cross, ccColor4F color = ccc4f (1, 1, 1, 1));

    static void   InterSect (vector<Hexagon>& des,const vector<Hexagon>& src );
    static bool	  Find (const vector<Hexagon>& src, const Hexagon& target);

};
