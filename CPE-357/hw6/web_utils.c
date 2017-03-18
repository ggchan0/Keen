#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <fcntl.h>

#define QUEUE_LIMIT 10
#define KB 1024

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
      exit(-1);
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
      exit(-1);
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
         free(temp->data);
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

int hasReadAccess(struct stat stat_buf) {
   return stat_buf.st_mode & S_IRUSR || \
   stat_buf.st_mode & S_IRGRP || \
   stat_buf.st_mode & S_IROTH;
}

int hasExecAccess(struct stat stat_buf) {
   return stat_buf.st_mode & S_IXUSR || \
   stat_buf.st_mode & S_IXGRP || \
   stat_buf.st_mode & S_IXOTH;
}

void write200Head(int fd, int content_length) {
   char buf[KB];
   buf[0] = 0;
   snprintf(buf, KB, "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n"
                     "Content-Length: %d\r\n\r\n", content_length);
   int length = strlen(buf);
   write(fd, buf, length);
}

void throw400Error(int fd) {
   char *response = "HTTP/1.0 400 Bad Request\r\nContent-Type: text/html\r\n"
                     "Content-Length: 53\r\n\r\n"
                     "400 - Request could not be understood by the server.\n";
   int length = strlen(response);
   if (write(fd, response, length) < 0) {
      perror("");
   }
   exit(-1);
}

void throw403Error(int fd) {
   char *response = "HTTP/1.0 403 Permission Denied\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: 50\r\n\r\n"
                     "403 - Access to the following resource is denied.\n";
   int length = strlen(response);
   if (write(fd, response, length) < 0) {
      perror("");
   }
   exit(-1);
}

void throw404Error(int fd) {
   char *response = "HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\n"
                     "Content-Length: 26\r\n\r\n"
                     "404 - Resource not found.\n";
   int length = strlen(response);
   if (write(fd, response, length) < 0) {
      perror("");
   }
   exit(-1);
}

void throw500Error(int fd) {
   char *response = "HTTP/1.0 500 Internal Error\r\nContent-Type: text/html\r\n"
                     "Content-Length: 46\r\n\r\n"
                     "500 - Server encountered an unexpected error.\n";
   int length = strlen(response);
   if (write(fd, response, length) < 0) {
      perror("");
   }
   exit(-1);
}

void throw501Error(int fd) {
   char *response = "HTTP/1.0 501 Not Implemented\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: 33\r\n\r\n"
                     "501 - HTTP method not supported.\n";
   int length = strlen(response);
   if (write(fd, response, length) < 0) {
      perror("");
   }
   exit(-1);
}
