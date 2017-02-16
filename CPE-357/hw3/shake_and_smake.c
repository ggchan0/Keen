#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include "smake_structures.h"
#include "text_utils.h"

char *handleArgs(int argc, char ** argv) {
   if (argc > 2) {
      fprintf(stderr, "Too many arguments supplied, quitting\n");
      exit(EXIT_FAILURE);
   } else if (argc == 2) {
      return argv[1];
   } else {
      return NULL;
   }
}

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
   return difftime(buf2.st_mtime, buf1.st_mtime) > 0;
}

void executeCommands(Nodelist *commands) {
   CharNode *command = commands->head;
   while (command != NULL) {
      printf("%s\n", command->data);
      if (system(command->data) == -1) {
         fprintf(stderr, "Error executing command %s, quitting\n", command->data);
      }
      command = command->next;
   }
}

void processRule(Rulelist *rules, RuleNode *rule) {
   int execute_rule = 0;
   if (rule->dependencies->head == NULL) {
      execute_rule = 1;
   } else if (fileExists(rule->name) == 0) {
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
         } else if (recursive_rule == NULL) {
            fprintf(stderr, "File: %s not found, quitting\n", dependency->data);
            exit(EXIT_FAILURE);
         }
         dependency = dependency->next;
      }
   }
   if (execute_rule) {
      executeCommands(rule->commands);
   } else {
      printf("Smake: `%s` is up to date.\n", rule->name);
   }
}

void applyRules(Rulelist *rules, char *rule_name) {
   if (rules->head == NULL) {
      fprintf(stderr, "No valid targets, quitting\n");
      exit(EXIT_FAILURE);
   } else if (rule_name != NULL) {
      RuleNode *rule = getTarget(rules, rule_name);
      processRule(rules, rule);
   } else {
      processRule(rules, rules->head);
   }
}

int main(int argc, char ** argv) {
   char *file_name;
   Nodelist *master_list, *copied_list;
   Rulelist *rules;
   char *rule_name;
   file_name = "Smakefile";
   if (fileExists(file_name) == 0) {
      fprintf(stderr, "Missing smakefile, quitting\n");
      exit(EXIT_FAILURE);
   }
   rule_name = handleArgs(argc, argv);
   master_list = parseSmakefile(file_name);
   copied_list = copyNodelist(master_list);
   rules = getRules(copied_list);
   applyRules(rules, rule_name);
   freeAllData(rules, master_list, copied_list);
   return 0;
}
