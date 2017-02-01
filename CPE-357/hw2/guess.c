#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text_utils.h"
#include "utilitrees.h"

#define LEFT 0
#define RIGHT 1

char *pickArticle(char *str) {
   char *article = malloc(sizeof(char) * 3);
   switch(str[0]) {
      case 'A':
         article = "an\0";
         break;
      case 'a':
         article = "an\0";
         break;
      case 'E':
         article = "an\0";
         break;
      case 'e':
         article = "an\0";
         break;
      case 'I':
         article = "an\0";
         break;
      case 'i':
         article = "an\0";
         break;
      case 'O':
         article = "an\0";
         break;
      case 'o':
         article = "an\0";
         break;
      case 'U':
         article = "an\0";
         break;
      case 'u':
         article = "an\0";
         break;
      default:
         article = "a\0";
         break;
   }
   return article;
}

char *initializeAnswer() {
   char *input;
   do {
      printf("What is it (with article)? ");
      input = readline(stdin);
   } while (!input || isEmptyInput(input));
   return input;
}

char *confirmAnswer(char *answer) {
   char *input;
   char *article = pickArticle(answer);
   do {
      printf("Is it %s %s? ", article, answer);
      input = readline(stdin);
   } while (!input || isEmptyInput(input));
   free(article);
   return input;
}

char *promptForQuestion(char *answer_1, char *answer_2) {
   char *input;
   char *article_1 = pickArticle(answer_1);
   char *article_2 = pickArticle(answer_2);
   do {
      printf("What is a yes/no question"
             "that will distinguish %s %s from %s %s? ",
             article_1, answer_1, article_2, answer_2);
      input = readline(stdin);
   } while (!input || isEmptyInput(input));
   free(article_1);
   free(article_2);
   return input;
}

char *getAnswerToQuestion(char *answer) {
   char *input;
   char *article = pickArticle(answer);
   do {
      printf("What is the answer to the question for %s %s? ", article, answer);
      input = readline(stdin);
   } while (!input || isEmptyInput(input));
   free(article);
   return input;
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
   char *y_n_answer;
   char *new_question;
   char *new_answer;
   Treenode *new_answer_node;
   Treenode *new_question_node;
   int last_direction = -1;
   fclose(file);
   if (!isGoodTree(root)) {
      fprintf(stderr, "Tree is not valid, please give a valid qa.db file\n");
      exit(EXIT_FAILURE);
   } else {
      Treenode *temp = root;
      Treenode *prev = NULL;
      while (!hasNoChildren(temp)) {

      }
      y_n_answer = confirmAnswer(temp->data);
      if (isYes(y_n_answer)) {
         printf("My, am I clever. :)\nThanks for playing!\n");
      } else {
         printf("How disappointing.\n");
         free(y_n_answer);
         new_answer = initializeAnswer();
         new_answer_node = initializeNode(new_answer);
         new_question = promptForQuestion(new_answer, temp->data);
         new_question_node = initializeNode(new_question);
         y_n_answer = getAnswerToQuestion(new_answer);
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
            new_question_node->right = new_answer_node;
            new_question_node->left = temp;
         } else {
            new_question_node->right = temp;
            new_question_node->left = new_answer_node;
         }
         free(y_n_answer);
         file = fopen("qa.db", "w");
         writeToFile(root, file);
         fclose(file);
      }
   }
   freeTree(root);
}

void handleEmptyDB() {
   char *ans;
   Treenode *root;
   FILE *file;
   printf("qa.db: No such file or directory\n");
   printf("Unable to read database qa.db. Starting fresh.\n");
   ans = initializeAnswer();
   root = checkedTreenodeMalloc();
   root->data = ans;
   file = fopen("qa.db", "w");
   writeToFile(root, file);
   fclose(file);
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
