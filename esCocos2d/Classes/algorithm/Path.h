#pragma  once
//  Created by ZK on 14-5-29.


#include <set>
#include <assert.h>
#include <list>
#include <stack>
#include <queue>

class OrthoList;
class OrthoNode;


//non multi-thread safe
class Graph
{
public:
    enum eConst
    {
        OK,
        NODE_NOT_EXSIT,
        NODE_NOT_VALID,
        DEAD_END,
        INVALID_VALUE=INT_MAX
    };
    Graph() ;
    ~Graph();
    Graph::eConst	findShortestPath (int nodename_from, int nodename_to, std::list<int >& shortestPath/*out*/);
    void			BFS (int nodename);
    void			DFS (int nodename);
    void			clear();
    bool			SetNodeValidate (int nodename, bool validate);


    OrthoList*		_OL;

private:
    //return smallest weight edge adjacent node
    OrthoNode*		updateNodeAroundChoosen (std::vector<OrthoNode*>& choosen_nodes_);
    eConst			_findShortestPath (OrthoNode* from_, OrthoNode* to_, std::list<int>& shortestPath/*out*/);
    eConst			findAround (OrthoNode* n, bool check_chosen, std::set<OrthoNode*>& around/*out*/);
    void			BFS (std::queue<OrthoNode*>& ns, OrthoNode* n);
    void			DFS (std::stack<OrthoNode*>& ns, OrthoNode* n);
    void			access_node (OrthoNode* n);
    OrthoNode*		findNode (int nodename);



};
