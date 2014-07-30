//  Created by ZK on 14-5-29.
#pragma once

class OrthoNode;
class OrthoEdge
{
public:
    OrthoNode*		fromNode;
    OrthoNode*		toNode;
    OrthoEdge*		nextInEdge;
    OrthoEdge*		nextOutedge;
    int				weight;

    OrthoEdge (OrthoNode*		fromNode_,
               OrthoNode*		toNode_,
               OrthoEdge*		nextInEdge_,
               OrthoEdge*		nextOutedge_) :
        fromNode (fromNode_),
        toNode (toNode_),
        nextInEdge (nextInEdge_),
        nextOutedge (nextOutedge_),
        weight (0)
    {

    }
};