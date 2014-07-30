//  Created by ZK on 14-5-29.
#include "OrthoNode.h"
#include "OrthoEdge.h"
#include <stdio.h>
#include <limits.h>
OrthoNode::~OrthoNode()
{
    OrthoEdge* now = _nextOut;
    while (now)
    {
        OrthoEdge* next = now->nextOutedge;
        SAFE_DELETE (now);
        now = next;
    }
}

void OrthoNode::AddNextOut (OrthoEdge* next)
{
    AddNextOut (&_nextOut, next);
}

void OrthoNode::AddNextOut (OrthoEdge** e, OrthoEdge* next)
{
    if (*e == 0)
        *e = next;
    else
        AddNextOut (& ((*e)->nextOutedge), next);
}

void OrthoNode::AddNextIn (OrthoEdge* next)
{
    AddNextIn (&_nextIn, next);
}

void OrthoNode::AddNextIn (OrthoEdge** e, OrthoEdge* next)
{
    if (*e == 0)
        *e = next;
    else
        AddNextIn (& ((*e)->nextInEdge), next);
}

void OrthoNode::init()
{
    _parent=0;
    _distance_from_start=INT_MAX;
    _choosen=false;

}
