#ifndef UTILITREES
#define UTILITREES

typedef struct {
   char *data;
   struct Treenode *left;
   struct Treenode *right;
} Treenode;

Treenode *checkedTreenodeMalloc();

void writeToFile(Treenode *tree, FILE *file);

Treenode *readRootFromFile(FILE *file);

void preorder(Treenode *tree);

void freeTree(Treenode *tree);

int isBadTree(Treenode *tree);

#endif
