#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text_utils.h"
#include "utilitrees.h"

#define LEFT 0
#define RIGHT 1

int dbExists() {
   FILE *file = fopen("qa.db", "r");
   if (file) {
      fclose(file);
      return 1;
   } else {
      return 0;
   }
}

void updateDatabase(Treenode *root) {
   FILE *file = fopen("qa.db", "w");
   writeToFile(root, file);
   fclose(file);
}

Treenode *handleLoss(Treenode *root, Treenode *prev, Treenode *temp, int last_direction) {
   char *new_answer = initializeAnswer();
   char *new_question = promptForNewQuestion(new_answer, temp->data);
   Treenode *new_question_node = initializeNode(new_question);
   char *y_n_answer = getAnswerToNewQuestion(new_answer);
   if (prev == NULL) {
      root = new_question_node;
   } else {
      if (last_direction == LEFT) {
         prev->left = new_question_node;
      } else {
         prev->right = new_question_node;
      }
   }
   if (isYes(y_n_answer)) {
      new_question_node->right = initializeNode(new_answer);
      new_question_node->left = temp;
   } else {
      new_question_node->right = temp;
      new_question_node->left = initializeNode(new_answer);
   }
   free(y_n_answer);
   updateDatabase(root);
   printf("I'll get it next time, I'm sure.\nThanks for playing!\n");
   return root;
}

void handleExistingDB() {
   FILE *file = fopen("qa.db", "r");
   Treenode *root = readRootFromFile(file);
   char *y_n_answer = NULL;
   Treenode *temp = root;
   Treenode *prev = NULL;
   int last_direction = -1;
   fclose(file);
   if (!isGoodTree(root) || root == NULL) {
      fprintf(stderr, "Database is corrupt, please give a valid qa.db file\n");
      exit(EXIT_FAILURE);
   } else {
      while (!hasNoChildren(temp)) {
         y_n_answer = getAnswerToQuestion(temp->data);
         prev = temp;
         if (isYes(y_n_answer)) {
            last_direction = RIGHT;
            temp = temp->right;
         } else {
            last_direction = LEFT;
            temp = temp->left;
         }
         free(y_n_answer);
      }
      y_n_answer = confirmAnswer(temp->data);
      if (isYes(y_n_answer)) {
         printf("My, am I clever. :)\nThanks for playing!\n");
         free(y_n_answer);
      } else {
         printf("How disappointing.\n");
         free(y_n_answer);
         root = handleLoss(root, prev, temp, last_direction);
      }
   }
   freeTree(root);
}

void handleEmptyDB() {
   char *ans;
   Treenode *root;
   printf("qa.db: No such file or directory\n");
   printf("Unable to read database qa.db. Starting fresh.\n");
   ans = initializeAnswer();
   root = initializeNode(ans);
   updateDatabase(root);
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
