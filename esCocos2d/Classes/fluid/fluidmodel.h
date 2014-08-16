#ifndef FLUIDMODEL_H
#define FLUIDMODEL_H

#include <algorithm>
namespace fluid
{

struct Line
{
    double x1, y1;
    double x2, y2;
};

struct Material
{

    Material (double m_,
              double rd_,
              double k_,
              double v_,
              double d_,
              double g_);

    double m;
    double rd;
    double k;
    double v;
    double d;
    double g;
};


struct Particle
{

    Particle();
    Particle (Material *mat_,
              double x_,
              double y_,
              double u_,
              double v_);

    Material *mat;

    double x;
    double y;
    double u;
    double v;

    double dudx;
    double dudy;
    double dvdx;
    double dvdy;
    unsigned cx;
    unsigned cy;

    double px[3];
    double py[3];
    double gx[3];
    double gy[3];
};


struct Node
{

    Node();

    void clear();

    double m;
    double d;
    double gx;
    double gy;
    double u;
    double v;
    double ax;
    double ay;
    bool   active;
};


class FluidModel
{

public:

    FluidModel (unsigned gsizeX_,
                unsigned gsizeY_,
                unsigned particlesX_,
                unsigned particlesY_);
    ~FluidModel();

    void setPressed (bool b);
    void setMovePos (double x, double y);

    void step();
    const Line * getLines();

private:

    Particle *   particles;
    unsigned     gsizeX;
    unsigned     gsizeY;
    unsigned     particlesX;
    unsigned     particlesY;
    Node **      active;
    unsigned     activeCount;
    Material     water;

    bool         pressed;
    bool         pressedprev;

    double       mx;
    double       my;
    double       mxprev;
    double       myprev;

    Node **      grid;

    Line *       lines;
};
}

#endif
