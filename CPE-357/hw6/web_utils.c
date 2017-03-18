#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

typedef struct CharNode {
   char *data;
   struct CharNode *next;
} CharNode;

typedef struct Nodelist {
   struct CharNode *head;
} Nodelist;

void addToNodelist(Nodelist *nodelist, CharNode *charNode) {
   if (nodelist->head == NULL) {
      nodelist->head = charNode;
   } else {
      CharNode *temp = nodelist->head;
      while (temp->next != NULL) {
         temp = temp->next;
      }
      temp->next = charNode;
   }
}

CharNode *initializeCharNode(char *data) {
   CharNode *node = malloc(sizeof(CharNode));
   if (node == NULL) {
      fprintf(stderr, "Error trying to malloc for a CharNode\n");
      exit(EXIT_FAILURE);
   } else {
      node->data = data;
      node->next = NULL;
   }
   return node;
}

Nodelist *initializeNodelist() {
   Nodelist *list = malloc(sizeof(Nodelist));
   if (list == NULL) {
      fprintf(stderr, "Error tring to malloc for a Nodelist\n");
      exit(EXIT_FAILURE);
   } else {
      list->head = NULL;
   }
   return list;
}

void freeNodelist(Nodelist *list) {
   if (list != NULL) {
      CharNode *head = list->head;
      while (head != NULL) {
         CharNode *temp = head;
         head = head->next;
         free(temp);
      }
   }
}

char *doubleSize(char *str, int length) {
   return realloc(str, length);
}

char *readline(FILE *file) {
   int length = 10;
   int index = 0;
   char *str = malloc(sizeof(char) * length);
   int c;
   while ((c = fgetc(file)) != EOF && c != '\n') {
      str[index++] = c;
      if (index == (length - 1)) {
         length *= 2;
         str = doubleSize(str, length);
      }
   }
   str[index] = '\0';
   if (c == EOF) {
      free(str);
      str = NULL;
   }
   return str;
}

int isValidMethod(char *method) {
   return strcmp(method, "GET") == 0 || strcmp(method, "HEAD") == 0;
}
