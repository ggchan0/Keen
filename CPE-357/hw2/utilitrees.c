#include <stdlib.h>

#define LEFT 0
#define RIGHT 1

typedef struct {
   char *data;
   struct Treenode *left;
   struct Treenode *right;
} Treenode;

typedef struct {
   struct Treenode *root;
} Tree;