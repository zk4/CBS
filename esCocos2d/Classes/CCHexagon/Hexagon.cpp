#include "Hexagon.h"
#include <cmath>
#include "HexCoordinate.h"
using namespace std;

Hexagon Hexagon::s_directions[Hexagon::eDirection::COUNT] =
{
    { 0, 0 }, { 1, 0 }, { 1, -1 }, { 0, -1 },
    { -1, 0 }, { -1, +1 }, { 0, 1 }
};


float Hexagon::Distance (const Hexagon& h) const
{
    return max (max (abs (q - h.q), abs (r - h.r)), abs (y() - h.y()));
}

Hexagon::Hexagon (float q_ /*= 0*/, float r_ /*= 0 */, ccColor4F color_ /*= ccc4f(1, 1, 1, 1)*/) : q (q_), r (r_), color (color_)
{
#ifdef _DEBUG
    _init = false;
#endif // _DEBUG

}

Hexagon::Hexagon (const Hexagon& h)
{
    *this = h;
#ifdef _DEBUG
    _init = false;
#endif // _DEBUG
}

Hexagon Hexagon::Towards (const Hexagon& target) const
{
    Hexagon dir = target - *this;
    dir.Round();
    dir.Normalize();
    /* for (int i = 0; i < eDirection::COUNT; ++i)
     {
         if (dir == s_directions[i])
             return eDirection (i);
     }*/
    return dir;
}

Hexagon& Hexagon::Move (eDirection dir, int times)
{
    *this += (s_directions[dir] * times);
    return *this;
}
void CGAffineToGL (const CCAffineTransform *t, GLfloat *m)
{
    m[2] = m[3] = m[6] = m[7] = m[8] = m[9] = m[11] = m[14] = 0.0f;
    m[10] = m[15] = 1.0f;
    m[0] = t->a;
    m[4] = t->c;
    m[12] = t->tx;
    m[1] = t->b;
    m[5] = t->d;
    m[13] = t->ty;
}

void Hexagon::Draw (HexCoordinate* coord_)
{
#ifdef _DEBUG
    if (!_init)
    {
        static char buffer[4];
        sprintf(buffer, "%i,%i", (int)q, (int)r);
        ttf = CCLabelTTF::create(buffer, "Helvetica", 32);

        ttf->retain();
        _init = true;
    }
#endif // _DEBUG

    CCPoint center = coord_->Hex2CCP (*this);
    int length = coord_->length;
    //sqrt (3.0f), 0, sqrt (3.0f) / 2.0f, -1.5f,0,0 }
    CCAffineTransform& matrix = coord_->matrix;
    CCAffineTransform af = { matrix.a / sqrt (3.0f), 0, 0, matrix.d / -1.5f, center.x, center.y };
    kmGLPushMatrix();
    kmMat4 transfrom4x4;
    CGAffineToGL (&af, transfrom4x4.mat);
    kmGLMultMatrix (&transfrom4x4);
    for (int i = 0; i < 6; ++i)
    {
        float start_radian = M_PI / 3 * i + (M_PI / 6);
        float end_radian = start_radian + M_PI / 3;
        ccDrawColor4F (color.r, color.g, color.b, color.a);
        ccDrawLine (
            ccp  ( cos (start_radian)*length, sin (start_radian)*length) ,
            ccp ( cos (end_radian)*length, sin (end_radian)*length)
        );
    }
    ccPointSize(33);

#ifdef _DEBUG
    ttf->visit();
#endif // _DEBUG

    kmGLPopMatrix();

}



Hexagon Hexagon::Round() const
{
    //DDA algorithm
    float rx = round (q);
    float ry = round (y());
    float rz = round (r);
    float x_diff = abs (rx - q);
    float y_diff = abs (ry - y());
    float z_diff = abs (rz - r);
    if ((x_diff > y_diff) && (x_diff > z_diff))
        rx = -ry - rz;
    else if (y_diff > z_diff)
        ry = -rx - rz;
    else
        rz = -rx - ry;
    return Hexagon (rx, rz);
}

void Hexagon::Integerlize()
{
    q = round (q);
    r = round (r);
}

void Hexagon::Normalize()
{
    if (q != 0)
        q /= abs (q);
    if (r != 0)
        r /= abs (r);
}

Hexagon Hexagon::Mirror (const Hexagon& mirror) const
{
    return Hexagon (mirror.q + mirror.q - q, mirror.r + mirror.r - r).Color (color);
}

Hexagon& Hexagon::Color (const ccColor4F& color)
{
    this->color = color;
    return *this;
}


