#ifndef UTILITREES_H
#define UTILITREES_H

int checkHidden(char *arg);

int checkPerms(char *arg);

int isFlag(char *arg);

int isAbsolutePath(char *arg);

int isHidden(char *arg);

char getUserExecuteBit(struct stat buf);

char getGroupExecuteBit(struct stat buf);

void printPermissions(struct stat buf, char c);

char getFileTypeChar(struct stat buf);

void printIndent(int level);

#endif
