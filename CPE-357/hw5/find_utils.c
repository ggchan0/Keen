#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

typedef struct Node {
   char *data;
   struct Node *next;
} Node;

typedef struct List {
   Node *head;
} List;

Node *initializeNode(char *data) {
   Node *node = malloc(sizeof(Node));
   if (node == NULL) {
      perror("");
      exit(-1);
   } else {
      node->data = strdup(data);
      node->next = NULL;
   }
   return node;
}

List *initializeList() {
   List *list = malloc(sizeof(List));
   if (list == NULL) {
      perror("");
      exit(-1);
   } else {
      list->head = NULL;
   }
   return list;
}

void freeList(List *l) {
   Node *temp = l->head;
   while (temp != NULL) {
      Node *n = temp;
      temp = temp->next;
      free(n->data);
      free(n);
   }
   free(l);
}

void insert(List *l, Node *n) {
   Node *temp;
   Node *previous;
   if (l->head == NULL)
   {
      l->head = n;
   }else if(strcmp(n->data, l->head->data) <= 0)
   {
      n->next = l->head;
      l->head = n;
   } else {
      previous = l->head;
      temp = l->head->next;
      while(temp != NULL && strcmp(n->data, temp->data) > 0)
      {
         previous = temp;
         temp = temp->next;
      }
      if(temp == NULL) {
         previous->next = n;
      } else {
         n->next = temp;
         previous->next = n;
      }
   }
}

int isFlag(char *s) {
   return s[0] == '-';
}

int parseArgs(int *will_print, int *will_exec, int *will_name,
               int *substitute, char *name, int argc, char **argv) {
   int i;
   int valid_exec = 0;
   if (argc < 3) {
      fprintf(stderr, "Minimum of three arguments needed, %d given\n", argc);
      exit(-1);
   }
   if (isFlag(argv[1])) {
      fprintf(stderr, "No file/directory provided, quitting\n");
      exit(-1);
   }
   for (i = 0; i < argc; i++) {
      if (strcmp("-print", argv[i]) == 0) {
         *will_print = i;
      } else if (strcmp("-exec", argv[i]) == 0) {
         *will_exec = i;
      } else if (strcmp("-name", argv[i]) == 0) {
         *will_name = i++;
         strcpy(name, argv[i]);
      } else if (*will_exec && strcmp("{}", argv[i]) == 0) {
         *substitute = 1;
      } else if (*will_exec && strcmp(";", argv[i]) == 0) {
         if (*will_exec + 1 < i) {
            valid_exec = i;
         }
      }
   }
   if (*will_exec && valid_exec == 0) {
      fprintf(stderr, "Not valid -exec usage, missing ; or a valid command;\n");
      exit(-1);
   } else if (*will_exec) {
      argv[valid_exec] = NULL;
   }
   if (*will_print == *will_exec || (*will_print > 0 && *will_exec > 0)) {
      return 0;
   } else {
      return 1;
   }
}

char getFileTypeChar(struct stat buf) {
   char c;
   if (S_ISBLK(buf.st_mode)) {
      c = 'b';
   } else if (S_ISCHR(buf.st_mode)) {
      c = 'c';
   } else if (S_ISDIR(buf.st_mode)) {
      c = 'd';
   } else if (S_ISFIFO(buf.st_mode)) {
      c = 'p';
   } else if (S_ISREG(buf.st_mode)) {
      c = '-';
   } else if (S_ISLNK(buf.st_mode)) {
      c = 'l';
   } else {
      c = 's';
   }
   return c;
}

int doNotFilter(int will_name, char *sub, char *main) {
   return will_name == 0 || strstr(main, sub) != NULL;
}

void execute(int start, int substitute,
            char *exec_name, char **argv) {
   pid_t pid;
   pid = fork();
   if (pid == 0) {
      if (substitute) {
         int i = 0;
         while (argv[i] != NULL) {
            if (strcmp(argv[i], "{}") == 0) {
               argv[i] = exec_name;
            }
            i++;
         }
      }
      execvp(argv[start], argv + start);
      perror("");
      exit(-1);
   }
   wait(NULL);
}

void processFile(int will_print, int will_exec,
                  int substitute, char *file_name, char ** argv) {
   if (will_print) {
      printf("%s\n", file_name);
   } else {
      execute(will_exec + 1, substitute, file_name, argv);
   }
}
