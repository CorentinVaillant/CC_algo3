/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Algorithmique 3
 Mathias Paulin (Mathias.Paulin@irit.fr)

 Header file for the implementation of the QuadTree data-structure.

 DURING THE TEST, DO NOT MODIFY THIS FILE

 */
/*-----------------------------------------------------------------*/
#ifndef __QUADTREE__H__
#define __QUADTREE__H__
#include <stdbool.h>

/*------------------------  Point  -----------------------------*/
typedef struct _point2D {
    float x;
    float y;
} Point;

Point create_point(float x, float y);


/*------------------------  Node  -----------------------------*/

/** opaque definition of type Node */
typedef struct _treeNode Node;

/*------------------  Public interface of TAD Node  -----------------------*/
/** Get the stored values on a leaf.
 * \@pre node_isleaf(n) == true;
 */
const Point* const node_values(const Node* const n, int* nbvalues);

/** Accessors to the 4 sons of the node n.
 * @pre node_isleaf(n) == false;
 * @param n
 * @return
 */
Node* const node_upleft(const Node* const n);
Node* const node_upright(const Node* const n);
Node* const node_downleft(const Node* const n);
Node* const node_downright(const Node* const n);

/**
 * Is Node n a leaf ?
 * @param n
 * @return
 */
bool node_isleaf(const Node* const n);


/*------------------  Public interface of quadtree  -----------------------*/

/*------------------------  Type  -----------------------------*/
/** Opaque definition of the type MaxHeapTree */
typedef struct _quadtree QuadTree;

/** Constructor : builds an empty Quadtree that could store no more than npoints per leaf.
 */
QuadTree* quadtree_create(int npoints, Point min, Point max);

/** Operator : is the tree empty ?
 * bstree_empty : BinarySearchTree -> boolean
 */
bool quadtree_empty(const QuadTree* t);

/*------------------------  TreeDictionary  -----------------------------*/

/** Constructor : add a new value to the BinarySearchTree.
 *
 * @param t
 * @param v
 */
void quadtree_add(QuadTree* t, Point p);

/*------------------------  TreeVisitors  -----------------------------*/

/** Functor with user data to be used with the several visitors that can run on trees.
 *  This functor receive as argument subtree from which the root node must be processes and an opaque
 *  pointer to user provided data.
 */
typedef void(*OperateFunctor)(const Node* const, void*);

/** Depth first, prefix visitor */
void quadtree_depth_prefix(const QuadTree* t, OperateFunctor f, void* userData);


#endif /* __QUADTREE__H__ */
