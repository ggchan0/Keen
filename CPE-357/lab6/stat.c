#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
/*#include <sys/types.h>*/

char getUserExecuteBit(struct stat buf) {
   if ((buf.st_mode & S_ISUID)) {
      if ((buf.st_mode & S_IXUSR)) {
         return 's';
      } else {
         return 'S';
      }
   }
   if ((buf.st_mode & S_IXUSR)) {
      return 'x';
   } else {
      return '-';
   }   
}

char getGroupExecuteBit(struct stat buf) {
   if ((buf.st_mode & S_ISUID)) {
      if ((buf.st_mode & S_IXGRP)) {
         return 's';
      } else {
         return 'S';
      }
   }
   if ((buf.st_mode & S_IXGRP)) {
      return 'x';
   } else {
      return '-';
   }   
}

void printPermissions(struct stat buf, char c) {
   printf("Access: ");
   printf("%c", c);
   printf((buf.st_mode & S_IRUSR) ? "r" : "-");
   printf((buf.st_mode & S_IWUSR) ? "w" : "-");
   /*printf((buf.st_mode & S_IXUSR) ? "x" : "-");*/
   printf("%c", getUserExecuteBit(buf));
   printf((buf.st_mode & S_IRGRP) ? "r" : "-");
   printf((buf.st_mode & S_IWGRP) ? "w" : "-");
   /*printf((buf.st_mode & S_IXGRP) ? "x" : "-");*/
   printf("%c", getGroupExecuteBit(buf));
   printf((buf.st_mode & S_IROTH) ? "r" : "-");
   printf((buf.st_mode & S_IWOTH) ? "w" : "-");
   printf((buf.st_mode & S_IXOTH) ? "x" : "-");
   printf("\n\n");
}

void printFileType(char c) {
   switch (c) {
      case 'b':
         printf("Type: Block special file\n");
         break;
      case 'c':
         printf("Type: Character special file\n");
         break;
      case 'd':
         printf("Type: Directory\n");
         break;
      case 'p':
         printf("Type: FIFO\n");
         break;
      case 'l':
         printf("Type: Symbolic link\n");
         break;
      case 's':
         printf("Type: Socket link\n");
         break;
      default:
         printf("Type: Regular file\n");
         break;
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


int main(int argc, char ** argv) {
   int i;
   struct stat buf;
   char type;
   for (i = 0; i < argc; i++) {
      if (stat(argv[i], &buf) < 0) {
         printf("%s does not exist.\n", argv[i]);
      } else {
         type = getFileTypeChar(buf);
         printf("File: '%s'\n", argv[i]);
         printFileType(type);
         printf("Size: %d\n", (int) buf.st_size);
         printf("Inode: %d\n", (int) buf.st_ino);
         printf("Links: %d\n", (int) buf.st_nlink);
         printPermissions(buf, type);
      }
   }
   return 0;
}
