#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text_utils.h"
#include "utilitrees.h"

#define LEFT 0
#define RIGHT 1

char *promptAnswer() {
   char *ans;
   do {
      printf("What is it (with article)?\n");
      ans = readline(stdin);
   } while (!ans || isEmptyInput(ans));
   return ans;
}

int dbExists() {
   FILE *file = fopen("qa.db", "r");
   if (file) {
      fclose(file);
      return 1;
   } else {
      return 0;
   }
}

void handleExistingDB() {
   FILE *file = fopen("qa.db", "r");
   Treenode *root = readRootFromFile(file);
   preorder(root);
   freeTree(root);
}

void handleEmptyDB() {
   char *ans;
   Treenode *root;
   FILE *file;
   printf("qa.db: No such file or directory\n");
   printf("Unable to read database qa.db. Starting fresh.\n");
   ans = promptAnswer();
   root = checkedTreenodeMalloc();
   root->data = ans;
   file = fopen("qa.db", "w");
   writeToFile(root, file);
   freeTree(root);
}

int main(void) {
   if (dbExists()) {
      handleExistingDB();
   } else {
      handleEmptyDB();
   }
   return 0;
}
