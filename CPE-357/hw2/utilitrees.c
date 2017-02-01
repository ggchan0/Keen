#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "text_utils.h"

#define LEFT 0
#define RIGHT 1

typedef struct Treenode {
   char *data;
   struct Treenode *left;
   struct Treenode *right;
} Treenode;

Treenode *initializeNode(char *data) {
   Treenode *node = (Treenode *) calloc(1, sizeof(Treenode));
   if (node == NULL) {
      fprintf(stderr, "Error tring to malloc for a treenode\n");
      exit(EXIT_FAILURE);
   } else {
      node->data = data;
      node->left = NULL;
      node->right = NULL;
   }
   return node;
}

void writeToFile(Treenode *tree, FILE *file) {
   if (tree == NULL) {
      fprintf(file, "\n");
   } else {
      fprintf(file, "%s\n", tree->data);
      writeToFile(tree->left, file);
      writeToFile(tree->right, file);
   }
}

Treenode *readRootFromFile(FILE *file) {
   char *str = readline(file);
   Treenode *tree = NULL;
   if (str != NULL && !isEmptyInput(str)) {
      tree = initializeNode(str);
      tree->left = readRootFromFile(file);
      tree->right = readRootFromFile(file);
   } else {
      free(str);
   }
   return tree;
}

void preorder(Treenode *tree) {
   if (tree != NULL) {
      printf("%s\n", tree->data);
      preorder(tree->left);
      preorder(tree->right);
   } else {
      printf("NULL\n");
   }
}

void freeTree(Treenode *tree) {
   if (tree != NULL) {
      freeTree(tree->left);
      freeTree(tree->right);
      free(tree->data);
      free(tree);
   }
}

int isGoodTree(Treenode *tree) {
   if (tree != NULL) {
      int is_full_tree = tree->left != NULL && tree->right != NULL;
      int is_leaf = tree->left == NULL && tree->right == NULL;
      if (is_full_tree || is_leaf) {
         return isGoodTree(tree->left) && isGoodTree(tree->right);
      } else {
         return 0;
      }
   } else {
      return 1;
   }
}

int hasNoChildren(Treenode *tree) {
   return tree->left == NULL && tree->right == NULL;
}
