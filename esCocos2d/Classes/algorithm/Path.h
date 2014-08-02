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
    T				data;
    OrthoNode<T>*	p;    //parent
    float			iF;   //distance_from_start


    bool			_closed;
    bool			_validate;

    inline bool  operator== (const OrthoNode<T>& n) const
    {
        return  this->data == n._data;
    }

    inline bool  operator!= (const OrthoNode<T>& n) const
    {
        return ! (*this == n);
    }

    OrthoNode (T data_,
               OrthoEdge<T>* nextOut_ = 0,
               OrthoEdge<T>* nextIn_ = 0) :
        data (data_),
        _nextIn (nextIn_),
        _nextOut (nextOut_),
        _validate (true)
    {
        init();
    }
    ~OrthoNode()
    {
        OrthoEdge<T>* now = _nextOut;
        while (now)
        {
            OrthoEdge<T>* next = now->nextOutedge;
            delete (now);
            now = next;
        }
    }

    inline void  AddNextOut (OrthoEdge<T>* next)
    {
        AddNextOut (&_nextOut, next);
    }

    inline void  AddNextOut (OrthoEdge<T>** e, OrthoEdge<T>* next)
    {
        if (*e == 0)
            *e = next;
        else
            AddNextOut (& ((*e)->nextOutedge), next);
    }

    inline void  AddNextIn (OrthoEdge<T>* next)
    {
        AddNextIn (&_nextIn, next);
    }

    inline void  AddNextIn (OrthoEdge<T>** e, OrthoEdge<T>* next)
    {
        if (*e == 0)
            *e = next;
        else
            AddNextIn (& ((*e)->nextInEdge), next);
    }

    void  init()
    {
        p = 0;
        iF = INT_MAX;

        _closed = false;

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
struct OrthoNodeComp
{
    bool operator() (const OrthoNode<T>*  lhs, const OrthoNode<T>* rhs) const
    {
        return lhs->data < rhs->data;
    }
};
template<typename T>
class OrthoList
{
public:

    std::set<OrthoNode<T> *, OrthoNodeComp<T> > nodes;

    inline void  addEdge (T from_data, T to_data, int weight)
    {
        //TODO: find duplicated

        OrthoNode<T> * f = findNode (from_data);
        if (!f)
        {
            f = new OrthoNode<T> (from_data, 0, 0);
            nodes.insert (f);
        }

        OrthoNode<T> * t = findNode (to_data);
        if (!t)
        {
            t = new OrthoNode<T> (to_data, 0, 0);
            nodes.insert (t);
        }

        OrthoEdge<T> * ef2t = findEdge (f, t);
        if (!ef2t)
        {
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

    inline OrthoNode<T>*  findNode (T node)
    {
        OrthoNode<T> n (node, NULL, NULL);
        auto ret = nodes.find (&n);
        if (ret != nodes.end())
            return *ret;
        return NULL;
    }

    inline OrthoEdge<T>*  findEdge (OrthoNode<T>* from, OrthoNode<T>* to)
    {
        assert (from && to);
        OrthoEdge<T>* e = from->get_nextOut();
        while (e)
        {
            if (e->toNode == to)
                return e;
            else
                e = e->nextOutedge;
        }
        return NULL;
    }

    ~OrthoList()
    {
        for (auto a : nodes)
        {
            delete (a);
        }
        nodes.clear();
    }

    inline void  initNodes()
    {
        for (auto a : nodes)
        {
            a->init();
        }
    }

    inline  void  addNEdge (T from, T to, int weight)
    {
        addEdge (from, to, weight);
        addEdge (to, from, weight);
    }

};

#ifdef MESSAGE_SUPPORT
#include "messaging/MessageDispatcher.h"
#endif // MESSAGE_SUPPORT
template<typename T>
class Graph
{
public:
    enum eConst
    {
        OK,
        NODE_NOT_EXSIT,
        NODE_NOT_VALID,
        DEAD_END,
        NO_MAP,
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
        for (auto a : _OL->nodes)
        {
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


    OrthoNode<T>*  findNode (T nodename)
    {
        return _OL->findNode (nodename);
    }

    eConst  _findShortestPath (OrthoNode<T>* from_, OrthoNode<T>* to_ )
    {
        //Dijkstra algorithm
        if (! (from_ &&  to_))
        {
            return NODE_NOT_EXSIT;
        }
        if (!from_->_validate || !to_->_validate)
        {
            return NODE_NOT_VALID;
        }
        return  Dijkstra (from_, to_ );

    }
    template<typename T>
    struct node_greater
            : public binary_function<OrthoNode<T>*, OrthoNode<T>*, bool>
    {
        // functor for operator>
        bool operator() (const OrthoNode<T>*  _Left, const OrthoNode<T>* _Right) const
        {
            return _Left->iF > _Right->iF || _Left->iF== _Right->iF &&  _Left <_Right;
        }
    };



public:

    eConst AStar (OrthoNode<T>* from_, OrthoNode<T>*  to_, std::function<float (T& t1, T& t2)> heuristic )
    {
        if (! (from_ && to_)) return NODE_NOT_VALID;
        if (_OL->nodes.empty())return NO_MAP;
        priority_queue<OrthoNode<T>*, vector<OrthoNode<T>*>, node_greater<T>> OpenList;
        set<OrthoNode<T>*> closedlist;
        from_->iF = heuristic (from_->data,to_->data);

        OpenList.push (from_);

        while (!OpenList.empty()  )
        {
            OrthoNode<T>* u = OpenList.top();
            closedlist.insert (u);

            OpenList.pop();
            OrthoEdge<T>*  edge = u->get_nextOut();
            while (edge)
            {
                auto v = edge->toNode;

                if (closedlist.find (v) == closedlist.end() &&  v->_validate )
                {
                    auto u = edge->fromNode;

                    v->iF = heuristic (v->data, to_->data) + edge->weight;
                    v->p = u;
#ifdef MESSAGE_SUPPORT
                    //   DD (Telegram_ACCESS_NODE, { (double) (int) (&v->data), (double) (int) (&u->data) });
#endif // MESSAGE_SUPPORT

                    OpenList.push (v);

                }

                edge = edge->nextOutedge;
            }
            if (u == to_) return OK ;

        }
        return DEAD_END;
    }
    void Dijkstra (OrthoNode<T>* from_, OrthoNode<T>*  to_=NULL)
    {
        if (_OL->nodes.empty())return ;
        priority_queue<OrthoNode<T>*, vector<OrthoNode<T>*>, node_greater<T>> Q;

        for (auto v : _OL->nodes)
        {
            if (!v->_validate)continue;
            v->d = INT_MAX;
            v->p = NULL;
            v->_closed = false;

        }

        from_->G = 0;
        Q.push (from_);
        while (!to_ && !Q.empty() || (!Q.empty() &&  !to_->_closed))
        {
            OrthoNode<T>* u = Q.top();
            Q.pop();
            OrthoEdge<T>*  edge = u->get_nextOut();
            while (edge )
            {
                auto v = edge->toNode;
                auto u = edge->fromNode;

                if (v->G > u->G + edge->weight)
                {
                    v->G = u->G + edge->weight;
                    v->p = u;
#ifdef MESSAGE_SUPPORT
                    //         DD (Telegram_ACCESS_NODE, { (double) (int) (&v->data), (double) (int) (&u->data) });
#endif // MESSAGE_SUPPORT
                    Q.push (v);

                }
                edge = edge->nextOutedge;
            }
            u->_closed = true;

        }

    }

    eConst  findAround (OrthoNode<T>* n, bool check_chosen, set<OrthoNode<T>*>& around/*out*/)
    {
        OrthoEdge<T>*  edge = n->get_nextOut();
        while (edge)
        {
            if (edge->toNode->_closed)
            {
                around.insert (edge->toNode);
            }
            edge = edge->nextOutedge;
        }
        return  eConst::OK;
    }


};
}