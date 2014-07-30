//  Created by ZK on 14-5-29.

#include "Path.h"
#include <limits.h>
#include <algorithm>
#include <list>
#include <queue>

using namespace std;
#include "OrthoEdge.h"
#include "OrthoNode.h"
#include "OrthoList.h"


struct node_greater
        : public binary_function<OrthoNode*, OrthoNode*, bool> {
    // functor for operator>
    bool operator()(const OrthoNode*  _Left, const OrthoNode* _Right) const
    {
        // apply operator> to operands
        return _Left->_distance_from_start > _Right->_distance_from_start;
    }
};

// update nodes around choosen_nodes,return smallest weighted node
OrthoNode* Graph::updateNodeAroundChoosen (vector<OrthoNode*>& choosen_nodes_)
{

    priority_queue<OrthoNode*, vector<OrthoNode*>, node_greater  > updated_nodes;

    for (auto choosen_node : choosen_nodes_) {
        OrthoEdge*  edge = choosen_node->get_nextOut();
        while (edge) {
            if (edge->weight == INVALID_VALUE) {
                edge = edge->nextOutedge;
                continue;
            }
            OrthoNode* toNode = edge->toNode;
            if (toNode->_choosen) {
                edge = edge->nextOutedge;
                continue;
            }
            if (!toNode->_validate) {
                edge = edge->nextOutedge;
                continue;
            }
            int nodefromstart = edge->weight + choosen_node->_distance_from_start;
            if (toNode->_distance_from_start > nodefromstart) {
                toNode->_parent = choosen_node;
                toNode->_distance_from_start = nodefromstart;
            }

            updated_nodes.push(toNode);
            edge = edge->nextOutedge;
        }
    }
    //find smallest
    if (updated_nodes.empty())return NULL;
    return updated_nodes.top();

}

OrthoNode* Graph::findNode (int nodename)
{
    return _OL->findNode (nodename);
}
Graph::eConst	Graph::findShortestPath (int nodename_from, int nodename_to, list<int >& shortestPath/*out*/)
{
    return _findShortestPath (_OL->findNode (nodename_from), _OL->findNode (nodename_to), shortestPath);
}
Graph::eConst Graph::_findShortestPath (OrthoNode* from_, OrthoNode* to_, list<int>& shortestPath/*out*/)
{
    //Dijkstra algorithm
    if (! (from_ &&  to_)) {
        return NODE_NOT_EXSIT;
    }
    if (!from_->_validate || !to_->_validate) {
        return NODE_NOT_VALID;
    }
    _OL->initNodes();
    static  vector<OrthoNode*> chosen_nodes;
    chosen_nodes.clear();
    from_->_distance_from_start = 0;
    from_->_choosen = true;
    chosen_nodes.push_back (from_);
    while (!to_->_choosen) {
        OrthoNode* node = updateNodeAroundChoosen (chosen_nodes);
        if (node == NULL) {
            return  DEAD_END;
        }
        node->_choosen = true;
        chosen_nodes.push_back (node);
    }
    //reconstruct result
    OrthoNode* nownode = to_;
    while (nownode) {
        shortestPath.insert (shortestPath.begin(), 1, nownode->_data);
        nownode = nownode->_parent;
    }
    return OK;
}



#ifndef SAFE_DELETE
#define SAFE_DELETE(p)   do { if(p) { delete (p); (p) = 0; } } while(0)
#endif
Graph::~Graph()
{
    clear();
}

void Graph::clear()
{
    SAFE_DELETE (_OL);
    _OL = new OrthoList;
}



void Graph::BFS (int nodename)
{
    _OL->initNodes();
    OrthoNode* n = findNode (nodename);
    std::queue<OrthoNode*> s;
    s.push (n);
    BFS (s, n);
}

void Graph::BFS (std::queue<OrthoNode*>& ns, OrthoNode* n)
{
    access_node (n);
    set<OrthoNode*> ss;
    findAround (n, true, ss);
    for (auto a : ss) {
        ns.push (a);
    }
    ns.pop();
    if (ns.size() <= 0)return;

    BFS (ns, ns.front());

}



void Graph::access_node (OrthoNode* n)
{
    if (!n->_choosen) {
        n->_choosen = true;
        printf ("%d\n", n->_data);
    }
}

void Graph::DFS (int nodename)
{
    _OL->initNodes();
    OrthoNode* n = findNode (nodename);
    std::stack<OrthoNode*> s;
    s.push (n);
    while (n != NULL) {
        DFS (s, n);
        s.pop();
        if (s.size() > 0) {
            n = s.top();
        } else {
            n = NULL;
        }
    }
}

void Graph::DFS (std::stack<OrthoNode*>& ns, OrthoNode* n)
{
    access_node (n);
    set<OrthoNode*> ss;
    findAround (n, true, ss);
    if (ss.size() <= 0)return;

    ns.push (*ss.begin());
    DFS (ns, *ss.begin());

}

Graph::eConst Graph::findAround (OrthoNode* n, bool check_chosen, set<OrthoNode*>& around/*out*/)
{
    OrthoEdge*  edge = n->get_nextOut();
    while (edge) {
        if (edge->toNode->_choosen) {
            around.insert (edge->toNode);
        }
        edge = edge->nextOutedge;
    }
    return  eConst::OK;
}



bool Graph::SetNodeValidate (int nodename, bool validate)
{
    OrthoNode* a = _OL->findNode (nodename);
    assert (a);
    bool old = a->_validate;
    a->_validate = validate;
    return old;
}

Graph::Graph()
{
    _OL = new OrthoList;
}


