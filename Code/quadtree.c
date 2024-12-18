#include "quadtree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*------------------------  Point  -----------------------------*/

Point create_point(float x, float y) {
    Point p = {x, y};
    return p;
}

/*------------------  Private representation of TAD Node  -------------------*/


/** opaque definition of type Node */
struct _treeNode {
    bool isleaf;
    Point min;
    Point max;
    union {
        struct {
            Point split;
            Node* upleft;
            Node* upright;
            Node* downleft;
            Node* downright;
        } childs;
        struct {
            Point* points;
            int nbpoints;
        } pointset;
    } data;
    Node* parent;
};

/*------------------  Public interface of TAD Node  -----------------------*/
/** Get the stored values on a leaf.
 * \@pre node_isleaf(n) == true;
 */
const Point* const node_values(const Node* const n, int* nbvalues) {
    assert(node_isleaf(n));
    *nbvalues = n->data.pointset.nbpoints;
    return n->data.pointset.points;
}

/** Accessors to the 4 sons of the node n.
 * @pre node_isleaf(n) == false;
 * @param n
 * @return
 */
Node* const node_upleft(const Node* const n) {
    assert(!node_isleaf(n));
    return n->data.childs.upleft;
}

Node* const node_upright(const Node* const n) {
    assert(!node_isleaf(n));
    return n->data.childs.upright;
}

Node* const node_downleft(const Node* const n) {
    assert(!node_isleaf(n));
    return n->data.childs.downleft;
}

Node* const node_downright(const Node* const n) {
    assert(!node_isleaf(n));
    return n->data.childs.downright;
}

Point node_splitpos(const Node* const n) {
    return n->data.childs.split;
}
/**
 * Is Node n a leaf ?
 * @param n
 * @return
 */
bool node_isleaf(const Node* const n) {
    return n->isleaf;
}


Node* node_create(Point min, Point max) {
    Node* n = malloc(sizeof(struct _treeNode));
    n->isleaf = true;
    n->min = min;
    n->max = max;
    n->data.pointset.nbpoints = 0;
    n->parent=NULL;
    return n;
}

int node_numpoints(const Node* n) {
    assert(node_isleaf(n));
    return n->data.pointset.nbpoints;
}

void node_add_point(Node* n, Point p) {
    assert(node_isleaf(n));
    ++(n->data.pointset.nbpoints);
    n->data.pointset.points = realloc(n->data.pointset.points, n->data.pointset.nbpoints* sizeof(Point));
    n->data.pointset.points[n->data.pointset.nbpoints-1] = p;
}

/*------------------  Private interface of TAD Node  -----------------------*/
void node_subdivide(Node* n);


/******************************************************************************/
/**                          Control start here                              **/
/******************************************************************************/
/**
 *  Nom         :
 *  Prenom      :
 *  Num Etud    :
 **/

/* Exercice 1 */
struct _quadtree {

};

QuadTree* quadtree_create(int npoints, Point min, Point max) {
}

bool quadtree_empty(const QuadTree* t) {
}


/* Exercice 2 et Exercice 4
 */
void quadtree_add(QuadTree* t, Point p) {
    /* Exercice 2 */

    /* Exercice 4 */

    /* Exercice 2 */
}

/* Exercice 3 */
void node_depth_prefix(const Node* n, OperateFunctor f, void* userData) {
}

/** Depth first, prefix visitor */
void quadtree_depth_prefix(const QuadTree* t, OperateFunctor f, void* userData){
}


/* Exercice 4 */
void node_subdivide(Node* n) {
    assert(node_isleaf(n));
}
