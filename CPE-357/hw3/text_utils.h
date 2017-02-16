#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

char *doubleSize(char *str, int length);

char *readline(FILE *file);

int fileExists(char * file_name);

char *trimwhitespace(char *str);

int isEmpty(char *line);

int isCommand(char *line);

int hasTarget(char *line);

int isValidRule(char *line);

#endif