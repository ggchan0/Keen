#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
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
   fclose(file);
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
   if ((dependency_line = strtok(NULL, colon)) != NULL) {
      dependency_line = trimwhitespace(dependency_line);
      token = strtok(dependency_line, space);
      while (token != NULL) {
         char *trimmed_token = trimwhitespace(token);
         if (isEmpty(trimmed_token) == 0) {
            CharNode *node = initializeCharNode(trimmed_token);
            addToNodelist(dependencies, node);
         }
         token = strtok(NULL, space);
      }
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
            fprintf(stderr, "Invalid rule \"%s\", quitting\n", trimmed_line);
            exit(EXIT_FAILURE);
         }
      }
      cur = cur->next;
   }
   if (rule != NULL) {
      rule->commands = commands;
      addToRulelist(list, rule);
   }
   return list;
}



RuleNode *getTarget(Rulelist *rules, char *target) {
   RuleNode *rule = rules->head;
   while (rule != NULL) {
      if (strcmp(rule->name, target) == 0) {
         return rule;
      }
      rule = rule->next;
   }
   return NULL;
}

int isNewer(char *target_name, char *dependency_name) {
   struct stat buf1;
   struct stat buf2;
   stat(target_name, &buf1);
   stat(dependency_name, &buf2);
   return &buf1.st_mtime < &buf2.st_mtime;
}

void executeCommands(Nodelist *commands) {
   CharNode *command = commands->head;
   while (command != NULL) {
      system(command->data);
      command = command->next;
   }
}

void processRule(Rulelist *rules, RuleNode *rule) {
   int execute_rule = 0;
   if (fileExists(rule->name) == 0) {
      execute_rule = 1;
   } else if (rule->dependencies->head == NULL) {
      execute_rule = 1;
   } else {
      CharNode *dependency = rule->dependencies->head;
      while (dependency != NULL) {
         RuleNode *recursive_rule = getTarget(rules, dependency->data);
         if (recursive_rule != NULL) {
            processRule(rules, recursive_rule);
         }
         if (fileExists(dependency->data)) {
            if (isNewer(rule->name, dependency->data)) {
               execute_rule = 1;
            }
         } else {
            fprintf(stderr, "File: %s not found, quitting\n", dependency->data);
            exit(EXIT_FAILURE);
         }
         dependency = dependency->next;
      }
   }
   if (execute_rule) {
      executeCommands(rule->commands);
   } else {
      printf("Nothing to do for target %s\n", rule->name);
   }
}

void processRules(Rulelist *rules) {
   RuleNode *rule = rules->head;
   while (rule != NULL) {
      if (rule->name == NULL || rule->dependencies == NULL) {
         executeCommands(rule->commands);
      }
      processRule(rules, rule);
      rule = rule->next;
   }
}

int main(int argc, char ** argv) {
   char *file_name;
   Nodelist *master_list, *copied_list;
   Rulelist *rules;
   file_name = getFileName(argc, argv);
   master_list = parseSmakefile(file_name);
   copied_list = copyNodelist(master_list);
   rules = getRules(copied_list);
   if (rules->head == NULL) {
      fprintf(stderr, "No valid targets, quitting\n");
      exit(EXIT_FAILURE);
   } else {
      processRules(rules);
   }
   freeAllData(rules, master_list, copied_list);
   return 0;
}
