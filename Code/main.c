#include "quadtree.h"
#include <stdio.h>
#include <memory.h>


/**
 * Print the value of a node.
 * @param t the tree node to output
 * @param userData unused pointer.
 */
void print_node(const Node* const n, void* userData) {
    FILE* file = (FILE *) userData;

    if (node_isleaf(n)) {
        int nbp;
        const Point *pts = node_values(n, &nbp);
        fprintf(file, "{ ");
        for (int i=0; i<nbp; ++i) {
            fprintf(file, "( %.2f %.2f ) ", pts[i].x, pts[i].y);
        }
        fprintf(file, "} ");
    }
}

/**
 * This function output one node using the <a href="https://www.graphviz.org/documentation/">dot</a> syntax.
 * A node must defined its shape and its links to the left and right subtrees.
 * If one of this subtree is NULL, a grey box with the label NIL is drawn.
 * @dot
 * digraph node_example {
 *      node [shape = record];
 *      parent [label="bstree_parent()"]
 *      treeroot [label="{{<parent>}|root|{<left>|<right>}}"];
 *      left [label="bstree_left()"];
 *      right [label="bstree_right()"];
 *      parent:s ->treeroot:parent:c [headclip=false, tailclip=false]
 *      treeroot:left:c -> left:n [headclip=false, tailclip=false]
 *      treeroot:right:c -> right:n [headclip=false, tailclip=false]
 * }
 * @enddot
 * @param t the tree node to draw.
 * @param userData the file to output the dot commands. Concretely, this parameter will be of type FILE * when the functor is called.
 */
 // style=dashed,
void node_to_dot(const Node* const n, void* userData) {
    FILE* file = (FILE*) userData;

    print_node(n, stdout);

    if (!node_isleaf(n)) {
        fprintf(file, "\tn%p [label=\"{{<upleft>|<upright>}|{<downleft>|<downright>}}\"];\n",
                (void *)n);

        fprintf(file, "\tn%p:upleft:c -> n%p:n [headclip=false, tailclip=false, color=blueviolet]\n",
                (void *)n, (void *)node_upleft(n));
        fprintf(file, "\tn%p:downleft:c -> n%p:n [headclip=false, tailclip=false, color=gold]\n",
                (void *)n, (void *)node_downleft(n));
        fprintf(file, "\tn%p:downright:c -> n%p:n [headclip=false, tailclip=false, color=coral]\n",
                (void *)n, (void *)node_downright(n));
        fprintf(file, "\tn%p:upright:c -> n%p:n [headclip=false, tailclip=false, color=burlywood]\n",
                (void *)n, (void *)node_upright(n));

    } else {
        int nbpts = 0;
        const Point* pts = node_values(n, &nbpts);
        if (nbpts == 0) {
            fprintf(file, "\tn%p [style=filled, fillcolor=dodgerblue, label=\"Empty\"];\n", (void *)n);
        } else {
            if (nbpts == 1)
                fprintf(file, "\tn%p [style=filled, fillcolor=aquamarine4, label=\"%d point\"];\n", (void *)n, nbpts);
            else
                fprintf(file, "\tn%p [style=filled, fillcolor=aquamarine4, label=\"%d points\"];\n", (void *)n, nbpts);
        }
    }

}


/** Main function for testing the Tree implementation.
 * The main function expects one parameter that is the file where values added to the tree, searched into the
 * tree and removed from the tree are to be read.
 *
 * This file must contain the following informations :
 * - on the first line, the parameters of the tree described by an int (num points on leaves), and 4 floats (min and max points)
 * - on the second line, the number of points to be added to the tree,
 * - on the third line, the points to be added, represented by two floats and separated by a space (or tab).
 *
 * The values will be added, searched and remove in the order they are read from the file.
 */
int main(int argc, char** argv) {

    if (argc < 2) {
        fprintf(stderr, "usage : %s filename\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");

    if (!input) {
        perror(argv[1]);
        return 1;
    }

    int n;
    fscanf(input, "%d", &n);
    Point min;
    Point max;
    fscanf(input, "%f %f", &min.x, &min.y);
    fscanf(input, "%f %f", &max.x, &max.y);


    QuadTree* theTree = quadtree_create(n, min, max);

    printf("QuadTree created : empty = %s.\n", quadtree_empty(theTree) ? "true" : "false");


    printf("Adding values to the tree.\n\t");

    fscanf(input, "%d", &n);

    for (int i = 0; i < n; ++i) {
        Point p;
        fscanf(input, "%f %f", &p.x, &p.y);
        quadtree_add(theTree, p);
    }
    printf("\nDone.\n");

    printf("The tree contains : \n\t");
    quadtree_depth_prefix(theTree, print_node, stdout);
    printf("\nDone.\n");


    char dotname[strlen(argv[1])+1];
    strncpy(dotname, argv[1], strlen(argv[1])+1);
    strncpy(dotname+strlen(dotname)-3, "dot", 3);
    printf("Exporting the tree to %s.\n\t", dotname);
    FILE* output = fopen(dotname, "w");
    fprintf(output, "digraph QuadTree {\n\tgraph [ranksep=0.75, nodesep = 0.75];\n\tnode [shape = record];\n\n");
    quadtree_depth_prefix(theTree, node_to_dot, output);
    fprintf(output, "\n}\n");
    fclose(output);
    printf("\nDone.\n");



    fclose(input);
    return 0;
}
