#ifndef FINDUTILS_H
#define FINDUTILS_H

typedef struct Node {
   char *data;
   struct Node *next;
} Node;

typedef struct List {
   Node *head;
} List;

Node *initializeNode(char *data);

List *initializeList();

void freeList(List *l);

void insert(List *l, Node *n);

int isFlag(char *s);

int parseArgs(int *will_print, int *will_exec, int *will_name,
               int *substitute, char *name, int argc, char **argv);

char getFileTypeChar(struct stat buf);

int doNotFilter(int will_name, char *sub, char *main);

void execute(int start, int substitute,
            char *exec_name, char **argv);

void processFile(int will_print, int will_exec,
                  int substitute, char *file_name, char ** argv);

#endif