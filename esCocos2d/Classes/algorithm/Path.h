#pragma  once

#include <set>
#include <assert.h>
#include <list>
#include <stack>
#include <queue>
#include <algorithm>


using namespace std;
namespace algorithm
{

template<typename T>
class OrthoEdge;

template<typename T>
class OrthoNode
{
public:
    T				_data;
    OrthoNode<T>*	_parent;
    int				_distance_from_start;
    bool			_choosen;
    bool			_validate;

    inline bool  operator== (const OrthoNode<T>& n) const
    {
        return  this->_data == n._data;
    }

    inline bool  operator!= (const OrthoNode<T>& n) const
    {
        return ! (*this == n);
    }

    OrthoNode (T data_,
               OrthoEdge<T>* nextOut_ = 0,
               OrthoEdge<T>* nextIn_ = 0) :
        _data (data_),
        _nextIn (nextIn_),
        _nextOut (nextOut_),
        _validate (true)
    {
        init();
    }
    ~OrthoNode()
    {
        OrthoEdge<T>* now = _nextOut;
        while (now) {
            OrthoEdge<T>* next = now->nextOutedge;
            delete (now);
            now = next;
        }
    }

    inline void  AddNextOut(OrthoEdge<T>* next)
    {
        AddNextOut (&_nextOut, next);
    }

    inline void  AddNextOut(OrthoEdge<T>** e, OrthoEdge<T>* next)
    {
        if (*e == 0)
            *e = next;
        else
            AddNextOut (& ((*e)->nextOutedge), next);
    }

    inline void  AddNextIn(OrthoEdge<T>* next)
    {
        AddNextIn (&_nextIn, next);
    }

    inline void  AddNextIn(OrthoEdge<T>** e, OrthoEdge<T>* next)
    {
        if (*e == 0)
            *e = next;
        else
            AddNextIn (& ((*e)->nextInEdge), next);
    }

    void  init()
    {
        _parent = 0;
        _distance_from_start = INT_MAX;
        _choosen = false;

    }

    inline OrthoEdge<T>*	get_nextOut()const
    {
        return _nextOut;
    }
    inline OrthoEdge<T>*	get_nextIn()const
    {
        return _nextIn;
    }
private:
    OrthoEdge<T>*	_nextOut;
    OrthoEdge<T>*	_nextIn;


};
template<typename T>
class OrthoEdge
{
public:
    OrthoNode<T>*		fromNode;
    OrthoNode<T>*		toNode;
    OrthoEdge<T>*		nextInEdge;
    OrthoEdge<T>*		nextOutedge;
    int				weight;

    OrthoEdge (OrthoNode<T>*		fromNode_,
               OrthoNode<T>*		toNode_,
               OrthoEdge<T>*		nextInEdge_,
               OrthoEdge<T>*		nextOutedge_) :
        fromNode (fromNode_),
        toNode (toNode_),
        nextInEdge (nextInEdge_),
        nextOutedge (nextOutedge_),
        weight (0)
    {

    }
};


template<typename T>
struct OrthoNodeComp {
    bool operator() (const OrthoNode<T>*  lhs, const OrthoNode<T>* rhs) const
    {
        return lhs->_data < rhs->_data;
    }
};
template<typename T>
class OrthoList
{
public:

    std::set<OrthoNode<T> *, OrthoNodeComp<T> > nodes;

    inline void  addEdge(T from_data, T to_data, int weight)
    {
        //TODO: find duplicated

        OrthoNode<T> * f = findNode (from_data);
        if (!f) {
            f = new OrthoNode<T> (from_data, 0, 0);
            nodes.insert (f);
        }

        OrthoNode<T> * t = findNode (to_data);
        if (!t) {
            t = new OrthoNode<T> (to_data, 0, 0);
            nodes.insert (t);
        }

        OrthoEdge<T> * ef2t = findEdge (f, t);
        if (!ef2t) {
            ef2t = new OrthoEdge<T> (f, t, NULL, NULL);
            ef2t->weight = weight;
            f->AddNextOut (ef2t);
            t->AddNextIn (ef2t);
        }
    }
    //
    //void  getInEdge (OrthoNode* node, std::vector<OrthoEdge*>& inEdges/*out*/)
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
    //void  getOutEdge (OrthoNode* node, std::vector<OrthoEdge*>& outEdges/*out*/)
    //{
    //    OrthoEdge* nextOut = node->get_nextOut();
    //    //TODO use iterator
    //    while (nextOut)
    //    {
    //        outEdges.push_back (nextOut);
    //        nextOut = nextOut->nextOutedge;
    //    }
    //}

    inline OrthoNode<T>*  findNode(T node)
    {
        OrthoNode<T> n (node, NULL, NULL);
        auto ret = nodes.find (&n);
        if (ret != nodes.end())
            return *ret;
        return NULL;
    }



    inline OrthoEdge<T>*  findEdge(OrthoNode<T>* from, OrthoNode<T>* to)
    {
        assert (from && to);
        OrthoEdge<T>* e = from->get_nextOut();
        while (e) {
            if (e->toNode == to)
                return e;
            else
                e = e->nextOutedge;
        }
        return NULL;
    }

    ~OrthoList()
    {
        for (auto a : nodes) {
            delete (a);
        }
        nodes.clear();
    }

    inline void  initNodes()
    {
        for (auto a : nodes) {
            a->init();
        }
    }

    inline  void  addNEdge(T from, T to, int weight)
    {
        addEdge (from, to, weight);
        addEdge (to, from, weight);
    }

};


template<typename T>
class Graph
{
public:
    enum eConst {
        OK,
        NODE_NOT_EXSIT,
        NODE_NOT_VALID,
        DEAD_END,
        INVALID_VALUE = INT_MAX
    };
    Graph::Graph()
    {
        _OL = new OrthoList<T>;
    }
    ~Graph()
    {
        delete (_OL);
    }
    void  clear()
    {
        for (auto a : _OL->nodes) {
            delete (a);
        }
        _OL->nodes.clear();
    }
    void  addNEdge (T from, T to, int weight)
    {
        _OL->addNEdge (from, to, weight);
    }
    void  addEdge (T from, T to, int weight)
    {
        _OL->addEdge (from, to, weight);
    }
    eConst	 findShortestPath (T nodename_from, T nodename_to, list<T >& shortestPath/*out*/)
    {
        return _findShortestPath (_OL->findNode (nodename_from), _OL->findNode (nodename_to), shortestPath);
    }
    void  BFS (T nodename)
    {
        _OL->initNodes();
        OrthoNode<T>* n = findNode (nodename);
        std::queue<OrthoNode<T>*> s;
        s.push (n);
        BFS (s, n);
    }
    void  DFS (T nodename)
    {
        _OL->initNodes();
        OrthoNode<T>* n = findNode (nodename);
        std::stack<OrthoNode<T>*> s;
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

    bool  SetNodeValidate (T& nodename, bool validate)
    {
        OrthoNode<T>* a = _OL->findNode (nodename);
        if (!a)return false;
        assert (a);
        bool old = a->_validate;
        a->_validate = validate;
        return old;
    }
    OrthoList<T>*		_OL;
private:


    // update nodes around choosen_nodes,return smallest weighted node
    inline OrthoNode<T>*  updateNodeAroundChoosen (vector<OrthoNode<T>*>& choosen_nodes_)
    {


        OrthoNode<T>*  smallest=NULL;
        for (auto  choosen_node : choosen_nodes_) {
            OrthoEdge<T>*  edge = choosen_node->get_nextOut();
            while (edge) {
                if (edge->weight == INVALID_VALUE) {
                    edge = edge->nextOutedge;
                    continue;
                }
                // OrthoNode<T>* toNode = edge->toNode;
                if (edge->toNode->_choosen) {
                    edge = edge->nextOutedge;
                    continue;
                }
                if (!edge->toNode->_validate) {
                    edge = edge->nextOutedge;
                    continue;
                }
                int nodefromstart = edge->weight + choosen_node->_distance_from_start;
                if (edge->toNode->_distance_from_start > nodefromstart) {
                    edge->toNode->_parent = choosen_node;
                    edge->toNode->_distance_from_start = nodefromstart;
                }

                if (!smallest)
                    smallest = edge->toNode;
                else {
                    if (smallest->_distance_from_start > edge->toNode->_distance_from_start) {
                        smallest = edge->toNode;
                    }
                }
                edge = edge->nextOutedge;
            }
        }

        return smallest;
    }

    OrthoNode<T>*  findNode (T nodename)
    {
        return _OL->findNode (nodename);
    }

    eConst  _findShortestPath (OrthoNode<T>* from_, OrthoNode<T>* to_, list<T>& shortestPath/*out*/)
    {
        //Dijkstra algorithm
        if (! (from_ &&  to_)) {
            return NODE_NOT_EXSIT;
        }
        if (!from_->_validate || !to_->_validate) {
            return NODE_NOT_VALID;
        }
        _OL->initNodes();
        static  vector<OrthoNode<T>* > chosen_nodes;
        chosen_nodes.clear();
        from_->_distance_from_start = 0;
        from_->_choosen = true;
        chosen_nodes.push_back (from_);
        while (!to_->_choosen) {
            OrthoNode<T>* node = updateNodeAroundChoosen (chosen_nodes);
            if (node == NULL) {
                return  DEAD_END;
            }
            node->_choosen = true;
            chosen_nodes.push_back (node);
        }
        //reconstruct result
        OrthoNode<T>* nownode = to_;
        while (nownode) {
            shortestPath.insert (shortestPath.begin(), 1, nownode->_data);
            nownode = nownode->_parent;
        }
        return OK;
    }


    void  BFS (std::queue<OrthoNode<T>*>& ns, OrthoNode<T>* n)
    {
        access_node (n);
        set<OrthoNode<T>*> ss;
        findAround (n, true, ss);
        for (auto a : ss) {
            ns.push (a);
        }
        ns.pop();
        if (ns.size() <= 0)return;

        BFS (ns, ns.front());

    }

    void  access_node (OrthoNode<T>* n)
    {
        if (!n->_choosen) {
            n->_choosen = true;
            printf ("%d\n", n->_data);
        }
    }

    void  DFS (std::stack<OrthoNode<T>*>& ns, OrthoNode<T>* n)
    {
        access_node (n);
        set<OrthoNode<T>*> ss;
        findAround (n, true, ss);
        if (ss.size() <= 0)return;

        ns.push (*ss.begin());
        DFS (ns, *ss.begin());

    }

    eConst  findAround (OrthoNode<T>* n, bool check_chosen, set<OrthoNode<T>*>& around/*out*/)
    {
        OrthoEdge<T>*  edge = n->get_nextOut();
        while (edge) {
            if (edge->toNode->_choosen) {
                around.insert (edge->toNode);
            }
            edge = edge->nextOutedge;
        }
        return  eConst::OK;
    }


};
}