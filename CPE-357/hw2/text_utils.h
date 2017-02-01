#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

char *doubleSize(char *str, int length);

char *readline(FILE *file);

int isYes(char *str);

int isEmptyInput(char *str);

char *initializeAnswer();

char *confirmAnswer(char *answer);

char *promptForNewQuestion(char *answer_1, char *answer_2);

char *getAnswerToNewQuestion(char *answer);

char *getAnswerToQuestion(char *question);

#endif
