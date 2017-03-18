#ifndef WEB_UTILS
#define WEB_UTILS

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#define QUEUE_LIMIT 10
#define KB 1024

typedef struct CharNode {
   char *data;
   struct CharNode *next;
} CharNode;

typedef struct Nodelist {
   struct CharNode *head;
} Nodelist;

void addToNodelist(Nodelist *nodelist, CharNode *charNode);

CharNode *initializeCharNode(char *data);

Nodelist *initializeNodelist();

void freeNodelist(Nodelist *list);

char *doubleSize(char *str, int length);

char *readline(FILE *file);

int isValidMethod(char *method);

int hasReadAccess(struct stat stat_buf);

int hasExecAccess(struct stat stat_buf);

void write200Head(int fd, int content_length);

void throw400Error(int fd);

void throw403Error(int fd);

void throw404Error(int fd);

void throw500Error(int fd);

void throw501Error(int fd);

#endif
