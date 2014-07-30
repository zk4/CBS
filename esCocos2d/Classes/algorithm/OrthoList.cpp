//  Created by ZK on 14-5-29.
#pragma  once
#include "OrthoList.h"
#include "OrthoEdge.h"
#include "OrthoNode.h"
#include <assert.h>
#include <algorithm>
using namespace std;
void OrthoList::addEdge (int from_data, int to_data, int weight )
{
//TODO: find duplicated

    OrthoNode * f = findNode (from_data);
    if (!f)
    {
        f = new OrthoNode (from_data, NULL, NULL);
        nodes.insert (f);
    }

    OrthoNode * t = findNode (to_data);
    if (!t)
    {
        t = new OrthoNode (to_data, NULL, NULL);
        nodes.insert (t);
    }

    OrthoEdge * ef2t = findEdge (f, t);
    if (!ef2t)
    {
        ef2t=new OrthoEdge (f, t, NULL, NULL);
        ef2t->weight=weight;
        f->AddNextOut (ef2t);
        t->AddNextIn (ef2t);
    }
}
//
//void OrthoList::getInEdge (OrthoNode* node, std::vector<OrthoEdge*>& inEdges/*out*/)
//{
//    OrthoEdge* nextIn = node->get_nextIn();
//    //TODO use iterator
//    while (nextIn)
//    {
//        inEdges.push_back (nextIn);
//        nextIn = nextIn->nextInEdge;
//    }
//}
//
//
//void OrthoList::getOutEdge (OrthoNode* node, std::vector<OrthoEdge*>& outEdges/*out*/)
//{
//    OrthoEdge* nextOut = node->get_nextOut();
//    //TODO use iterator
//    while (nextOut)
//    {
//        outEdges.push_back (nextOut);
//        nextOut = nextOut->nextOutedge;
//    }
//}

OrthoNode* OrthoList::findNode (int node)
{
//speed it up !!!!!!!!!!!!!!!
    /*  for (auto a : nodes)
      {
          if (a->_data == node)
              return a;
      }
      return NULL;*/

    static OrthoNode n (0,NULL,NULL);
    n._data = node;
    auto ret = nodes.find (&n);
    if (ret!=nodes.end())
        return *ret;
    return NULL;
}



OrthoEdge* OrthoList::findEdge (OrthoNode* from, OrthoNode* to)
{
    assert (from && to);
    OrthoEdge* e=	from->get_nextOut();
    while (e)
    {
        if (e->toNode==to)
            return e;
        else
            e=e->nextOutedge;
    }
    return NULL;
}

OrthoList::~OrthoList()
{
    for (auto a : nodes)
    {
        SAFE_DELETE (a);
    }
    nodes.clear();
}

void OrthoList::initNodes()
{
    for (auto a: nodes)
    {
        a->init();
    }
}

void OrthoList::addNEdge (int from, int to, int weight)
{
    addEdge (from,to,weight);
    addEdge (to, from, weight);
}
