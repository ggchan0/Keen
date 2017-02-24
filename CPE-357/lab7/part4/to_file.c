#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
   char *file_name[1];
   char *args[2];
   char buf[1024];
   int fd;
   if (argc != 3) {
      fprintf(stderr, "Program requires 3 command line arguments, %d given\n", argc);
      exit(EXIT_FAILURE);
   }
   file_name[0] = argv[2];
   args[0] = argv[2];
   args[1] = NULL;
   buf[0] = '\0';
   strcat(buf, "./");
   strcat(buf, argv[1]);
   fd = open(file_name[0], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);
   dup2(fd, 1);
   execvp(buf, args);
   return 0;
}
