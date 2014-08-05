#include "BeeHive.h"

void BeeHive::MakeSolidHex (int radius, const Hexagon& center, vector<Hexagon>& hexagons, ccColor4F color)
{
    for (int q = radius; q >= -radius; --q)
    {
        for (int r = -radius; r <= radius; ++r)
        {
            if (abs (r) <= radius && abs (q) <= radius && abs (-q - r) <= radius)
                hexagons.push_back (Hexagon (q, r, color) += center);
        }
    }
}

void BeeHive::MakeRingHex (int radius, const Hexagon& center, vector<Hexagon>& ring, ccColor4F color)
{
    for (int q = -radius; q <= radius; ++q)
    {
        for (int r = -radius; r <= radius; ++r)
        {
            if (max (max (abs (q), abs (r)), abs (-q - r)) == radius)
            {
                ring.push_back (Hexagon (q, r, color) += center);
            }
        }
    }
}

void BeeHive::MakeRingHexes (int inner, int outter, const Hexagon& center, vector<Hexagon>& rings, ccColor4F color)
{
    for (int i = inner; i <= outter; ++i)
    {
        MakeRingHex (i, center, rings, color);
    }
}



void BeeHive::MakeCross (int radius, const Hexagon& center, vector<Hexagon>& cross, ccColor4F color)
{
    cross.push_back (center);
    for (int i = -radius; i <= radius; ++i)
    {
        if (i == 0)continue;
        int x = i;
        int z = -i;
        int y = -x - z;
        cross.push_back (Hexagon (x, y, color) += center);
        cross.push_back (Hexagon (y, z, color) += center);
        cross.push_back (Hexagon (x, z, color) += center);
    }
}

void BeeHive::InterSect (vector<Hexagon>& des,const vector<Hexagon>& src )
{
    for (int i=des.size()-1; i>=0; --i)
    {
        for (auto &ss : src)
        {
            if (des[i] == ss)
            {
                des.erase (des.begin()+i);
                break;
            }
        }
    }
}

bool BeeHive::Find (const vector<Hexagon>& src, const Hexagon& target)
{
    for (auto & s : src)
    {
        if (s == target)
            return true;
    }
    return false;
}



void BeeHive::MakeLine (const Hexagon& start, const Hexagon& end, vector<Hexagon>& line, ccColor4F color /*= ccc4f(1, 1, 1, 1)*/)
{
    float N = start.Distance (end);
    if (N==0)return;
    for (int i = 0; i <= N; ++i)
    {
        Hexagon  h = (start * (1 - i / N) + end * i / N).Round();
        h.color = color;
        line.push_back (h);
    }
}

void BeeHive::MakeRect (int width, int height, const Hexagon& lb, vector<Hexagon>& rect, ccColor4F color /*= ccc4f (1, 1, 1, 1)*/)
{
    for (int h = 0; h < height; ++h)
    {
        for (int w = 0; w < width; ++w)
        {
            rect.push_back ({ lb.q + ((h+1) / 2) + w, lb.r - h, color });
        }
    }
}

void BeeHive::MakeRect2 (int width, int height, const Hexagon& lb, vector<Hexagon>& rect, ccColor4F color /*= ccc4f (1, 1, 1, 1)*/)
{
    for (int h = 0; h < height; ++h)
    {
        for (int w = 0; w < width - ( h & 1); ++w)
        {
            rect.push_back ({ lb.q + ((h + 1) / 2) + w, lb.r - h, color });
        }
    }
}








