#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "smake_structures.c"
#include "text_utils.c"

Nodelist *parseSmakefile(char *file_name) {
   FILE *file = fopen(file_name, "r");
   char *line;
   Nodelist *nodelist = initializeNodelist();
   while ((line = readline(file)) != NULL) {
      CharNode *node = initializeCharNode(line);
      addToNodelist(nodelist, node);
   }
   return nodelist;
}

char *getFileName(int argc, char ** argv) {
   char *file_name;
   if (argc > 2) {
      fprintf(stderr, "Too many arguments, quitting\n");
      exit(EXIT_FAILURE);
   } else if (argc == 2) {
      file_name = argv[1];
   } else {
      file_name = "Smakefile";
   }
   if (fileExists(file_name) == 1) {
      return file_name;
   } else {
      fprintf(stderr, "File not found, quitting\n");
      exit(EXIT_FAILURE);
   }
}

void createValidRule(RuleNode *rule, char *trimmed_line) {
   char *token = NULL;
   char *colon = ":";
   char *space = " ";
   char *rulename = NULL;
   char *dependency_line = NULL;
   Nodelist *dependencies = initializeNodelist();
   token = strtok(trimmed_line, colon);
   rulename = trimwhitespace(token);
   dependency_line = trimwhitespace(token);
   token = strtok(dependency_line, colon);
   while (token != NULL) {
      CharNode *node = initializeCharNode(trimwhitespace(token));
      addToNodelist(dependencies, node);
      token = strtok(NULL, space);
   }
   rule->name = rulename;
   rule->dependencies = dependencies;
}

Rulelist *getRules(Nodelist *lines) {
   Rulelist *list = initializeRulelist();
   RuleNode *rule = NULL;
   Nodelist *commands = NULL;
   CharNode *cur = lines->head;
   while (cur != NULL) {
      int isACommand = isCommand(cur->data);
      char *trimmed_line = trimwhitespace(cur->data);
      if (isEmpty(trimmed_line) == 0) {
         if (isACommand == 0 && isValidRule(trimmed_line)) {
            if (rule != NULL) {
               rule->commands = commands;
               commands = NULL;
            }
            addToRulelist(list, rule);
            rule = initializeRuleNode(NULL, NULL, NULL);
            if (hasTarget(trimmed_line)) {
               createValidRule(rule, trimmed_line);
            }
         } else if (isACommand) {
            CharNode *command = initializeCharNode(trimmed_line);
            if (commands == NULL) {
               commands = initializeNodelist();
            }
            addToNodelist(commands, command);
         } else {
            fprintf(stderr, "Invalid rule, quitting\n");
            exit(EXIT_FAILURE);
         }
      }
      cur = cur->next;
   }
   return list;
}

int main(int argc, char ** argv) {
   char *file_name;
   Nodelist *master_list, *copied_list;
   Rulelist *rules;
   file_name = getFileName(argc, argv);
   master_list = parseSmakefile(file_name);
   copied_list = copyNodelist(master_list);
   rules = getRules(copied_list);
   freeAllData(rules, master_list, copied_list);
   return 0;
}
