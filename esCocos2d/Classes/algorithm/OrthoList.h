//  Created by ZK on 14-5-29.
#pragma  once
#include <set>
#include "OrthoNode.h"

class OrthoEdge;

struct OrthoNodeComp
{
    bool operator() (const OrthoNode*  lhs, const OrthoNode* rhs) const
    {
        return lhs->_data < rhs->_data;
    }
};
class OrthoList
{
public:
    ~OrthoList();
    std::set<OrthoNode*, OrthoNodeComp> nodes;
    void initNodes();
    void addEdge (int from ,int to,int weight );
    void addNEdge (int from, int to, int weight);
    //void getInEdge (OrthoNode* node, std::vector<OrthoEdge*>&  inEdges/*out*/);
    //void getOutEdge (OrthoNode* node, std::vector<OrthoEdge*>&  outEdges/*out*/);

    OrthoNode* findNode (int node);

    OrthoEdge* findEdge (OrthoNode* from, OrthoNode* to);
};
