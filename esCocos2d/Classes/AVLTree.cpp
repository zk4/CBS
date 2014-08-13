// AVLTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cmath>
struct Node {
    int		data;
    Node*	l;
    Node*	r;
    Node*   p;

};

struct BinaryTree {
    Node**  _tree;
    Node*	_root;
    BinaryTree() : _tree ( NULL ) , _root ( NULL ) {}
    ~BinaryTree()
    {

    }
    void addNode ( int data )
    {

        bool bAdded = false;
        addNode ( _tree, data, bAdded );

    }
    void addNode ( Node** tree, int data, bool& bAdded )
    {

        if ( bAdded ) return;

        if ( tree == NULL ) {

            bAdded = true;
            Node* n = new Node();
            n->data = data;
            if ( !_root ) {
                tree = &_root;
            }
            *tree =  n;

            return;
        }

        addNode ( & ( ( *tree )->l ), data, bAdded );
        addNode ( & ( ( *tree )->r ), data, bAdded );




    }
//law1: on level i has at most pow(2,i) -1 nodes(i>=1);
    static int getMostNodesOnLevel ( int iLevel )
    {
        return pow ( 2, iLevel - 1 );
    }
//law2: i depth has at most pow(2,i) -1 nodes (i>=1);
    static int getAllNodes ( int iDepth )
    {
        return pow ( 2, iDepth ) - 1;
    }
//law3: n0 indicates leaf counts (degree is 0) , n2  indicates node with degree 2, then , n0=n2+1
    static int getN0Count ( int n2 )
    {
        return  n2 + 1;
    }
    static int getN2Count ( int n0 )
    {
        return  n0 - 1;
    }
//law4: 具有n 个结点的完全二叉树的深度为log ( node_count ) / log ( 2 ) + 1;
    static int getCompleteTreeDepth ( int node_count )
    {
        return log ( node_count ) / log ( 2 ) + 1;
    }
//law5: 对拥有 total_node_count 个结点的完全二叉树而言
    static int getParentId ( int node_id )
    {
        return node_id / 2;
    }

    static bool hasLeftChild ( int node_id, int total_node_count, int & lc/*out*/ )
    {
        if ( 2 * node_id > total_node_count ) return false;
        lc = 2 * node_id;
        return true;
    }
    static bool hasRightChild ( int node_id, int total_node_count, int & rc/*out*/ )
    {
        if ( ( 2 * node_id + 1 ) > total_node_count ) return false;
        rc = 2 * node_id + 1;
        return true;
    }
    void postOrderTraverse()
    {

    }
    void postOrderTraverse ( Node* n )
    {
        if ( !n )
            return;
        postOrderTraverse ( n->l );
        postOrderTraverse ( n->r );
        printf ( "%d,", n->data );
    }
};


int _tmain ( int argc, _TCHAR* argv[] )
{
    BinaryTree bt;
    bt.addNode ( 1 );
    bt.addNode ( 2 );
    bt.addNode ( 3 );
    bt.addNode ( 4 );
    bt.addNode ( 5 );
    bt.addNode ( 6 );

    bt.postOrderTraverse();



    return 0;
}

