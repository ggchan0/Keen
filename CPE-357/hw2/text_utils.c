#include <stdio.h>
#include <stdlib.h>

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

int isYes(char *str) {
   return str[0] == 'y' || str[0] == 'Y';
}

int isEmptyInput(char *str) {
   return str[0] == '\0';
}


char *initializeAnswer() {
   char *input;
   do {
      printf("What is it (with article)? ");
      input = readline(stdin);
   } while (!input || isEmptyInput(input));
   return input;
}

char *confirmAnswer(char *answer) {
   char *input;
   do {
      printf("Is it %s? ", answer);
      input = readline(stdin);
   } while (!input || isEmptyInput(input));
   return input;
}

char *promptForNewQuestion(char *answer_1, char *answer_2) {
   char *input;
   do {
      printf("What is a yes/no question "
             "that will distinguish %s from %s? ",
             answer_1, answer_2);
      input = readline(stdin);
   } while (!input || isEmptyInput(input));
   return input;
}

char *getAnswerToNewQuestion(char *answer) {
   char *input;
   do {
      printf("What is the answer to the question for %s? ", answer);
      input = readline(stdin);
   } while (!input || isEmptyInput(input));
   return input;
}

char *getAnswerToQuestion(char *question) {
   char *input;
   do {
      printf("%s ", question);
      input = readline(stdin);
   } while (!input || isEmptyInput(input));
   return input;
}
