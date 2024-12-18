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
 *  Nom         : Vaillant
 *  Prenom      : Corentin
 *  Num Etud    : XX XX XX 
 **/

/* Exercice 1 */
struct _quadtree {
    int maxpoint;
    struct _treeNode* baseNode;
};

QuadTree* quadtree_create(int npoints, Point min, Point max) {
    //continuité de la mémoire
    QuadTree * qtree = malloc(sizeof(QuadTree) + sizeof(struct _treeNode));
    qtree->maxpoint = npoints;
    qtree->baseNode = (struct _treeNode*)(qtree + 1);
    qtree->baseNode->isleaf = true;
    qtree->baseNode->max = max;
    qtree->baseNode->min = min;
    qtree->baseNode->data.pointset.nbpoints = 0;
    qtree->baseNode->data.pointset.points =malloc(sizeof(Point));
    *qtree->baseNode->data.pointset.points = create_point(0.,0.); //?

    return qtree;

}

bool quadtree_empty(const QuadTree* t) {
    return t->baseNode->isleaf && t->baseNode->data.pointset.nbpoints == 0;
}


/* Exercice 2 et Exercice 4
 */
void quadtree_add(QuadTree* t, Point p) {
    Node *curNode = t->baseNode;

    /* Exercice 2 */
    while (!(curNode->isleaf) ){
        
        const Point split = node_splitpos(curNode);
        if(split.x < p.x){ //on se place à droite
            if(split.y < p.y) // on se place en bas
                curNode = curNode->data.childs.downright;
            else //on se place en haut
                curNode = curNode->data.childs.upright;
        }else{//on se place à gauche
            if(split.y < p.y) // on se place en bas
                curNode = curNode->data.childs.downleft;
            else //on se place en haut
                curNode = curNode->data.childs.upleft;
        }
        
    }
    printf("step 1 done\n");
    
    /* Exercice 4 */
    while (curNode->data.pointset.nbpoints >= t->maxpoint){
        node_subdivide(curNode);
        const Point split = node_splitpos(curNode);
        if(split.x < p.x){ //on se place à droite
            if(split.y < p.y) // on se place en bas
                curNode = curNode->data.childs.downright;
            else //on se place en haut
                curNode = curNode->data.childs.upright;
        }else{//on se place à gauche
            if(split.y < p.y) // on se place en bas
                curNode = curNode->data.childs.downleft;
            else //on se place en haut
                curNode = curNode->data.childs.upleft;
        }   
    }
    printf("step 2 done\n");

    /* Exercice 2 */
    node_add_point(curNode,p);
    
}

/* Exercice 3 */
void node_depth_prefix(const Node* n, OperateFunctor f, void* userData) {
    f(n,userData);
    if(n->isleaf) return;
    node_depth_prefix(n->data.childs.upright,f,userData);
    node_depth_prefix(n->data.childs.downright,f,userData);
    node_depth_prefix(n->data.childs.downleft,f,userData);
    node_depth_prefix(n->data.childs.upleft,f,userData);
    
}

/** Depth first, prefix visitor */
void quadtree_depth_prefix(const QuadTree* t, OperateFunctor f, void* userData){
    node_depth_prefix(t->baseNode,f,userData);
}


/* Exercice 4 */
void node_subdivide(Node* n) {
    assert(node_isleaf(n));
    Point min = n->min;
    Point max = n->max;
    Point splitpos = create_point(
        (min.x + max.x)/2,
        (min.y + max.y)/2
    );

    Point middle_right  = create_point(max.x , splitpos.y);
    Point middle_down   = create_point(splitpos.x , min.y);
    Point middle_left   = create_point(min.x , splitpos.y);
    Point middle_up     = create_point(splitpos.x , max.y);

    Node * upright    = node_create(splitpos, max);
    Node * downright  = node_create(middle_down,middle_right);
    Node * downleft   = node_create(min,splitpos);
    Node * upleft     = node_create(middle_right,middle_up);

    for(int i = 0; i< n->data.pointset.nbpoints; i++){
        Point p = n->data.pointset.points[i];
        if(p.x < splitpos.x){
            if(p.y < splitpos.y)
                node_add_point(downleft,p);
            else
                node_add_point(upleft ,p);
        }else{
            if(p.y < splitpos.y)
                node_add_point(downright,p);
            else
                node_add_point(upright ,p);
        }
    }

    free(n->data.pointset.points);
    n->data.pointset.points = 0;
    n->isleaf = false;

    n->data.childs.split    = splitpos;

    n->data.childs.upright  = upright;
    n->data.childs.downright= downright;
    n->data.childs.downleft = downleft;
    n->data.childs.upleft   = upleft;
}
