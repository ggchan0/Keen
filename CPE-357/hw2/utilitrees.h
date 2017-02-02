#ifndef UTILITREES
#define UTILITREES

typedef struct Treenode {
   char *data;
   struct Treenode *left;
   struct Treenode *right;
} Treenode;

Treenode *initializeNode(char *data);

Treenode *checkedTreenodeMalloc();

void writeToFile(Treenode *tree, FILE *file);

Treenode *readRootFromFile(FILE *file);

void preorder(Treenode *tree);

void freeTree(Treenode *tree);

int isGoodTree(Treenode *tree);

int hasNoChildren(Treenode *tree);

#endif
