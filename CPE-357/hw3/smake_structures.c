#include <stdio.h>
#include <stdlib.h>

typedef struct CharNode {
   char *data;
   struct CharNode *next;
} CharNode;

typedef struct Nodelist {
   struct CharNode *head;
} Nodelist;

typedef struct RuleNode {
   char *name;
   struct Nodelist *dependencies;
   struct Nodelist *commands;
   struct RuleNode *next;
} RuleNode;

typedef struct Rulelist {
   struct RuleNode *head;
} Rulelist;

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

void addToRulelist(Rulelist *rulelist, RuleNode *ruleNode) {
   if (ruleNode == NULL) {
      return;
   }
   if (rulelist->head == NULL) {
      rulelist->head = ruleNode;
   } else {
      RuleNode *temp = rulelist->head;
      while (temp->next != NULL) {
         temp = temp->next;
      }
      temp->next = ruleNode;
   }
}

void printNodelist(Nodelist *list) {
   if (list != NULL) {
      CharNode *cur = list->head;
      printf("Printing char list\n");
      while (cur != NULL) {
         printf("%s\n", cur->data);
         cur = cur->next;
      }
   }
}

void printRuleNode(RuleNode *rule) {
   printf("Printing rule for %s\n", rule->name);
   printNodelist(rule->dependencies);
   printNodelist(rule->commands);
}

void printRulelist(Rulelist *list) {
   RuleNode* cur = list->head;
   printf("Printing rule list\n");
   while (cur != NULL) {
      printRuleNode(cur);
      cur = cur->next;
   }
}

int noTarget(RuleNode *node) {
   return node->name == NULL;
}

CharNode *initializeCharNode(char *data) {
   CharNode *node = malloc(sizeof(CharNode));
   if (node == NULL) {
      fprintf(stderr, "Error trying to malloc for a CharNode\n");
      exit(EXIT_FAILURE);
   } else {
      node->data = data;
      node->next = NULL;
   }
   return node;
}

RuleNode *initializeRuleNode(char *name,
                              Nodelist *dependencies, Nodelist *commands) {
   RuleNode *node = malloc(sizeof(RuleNode));
   if (node == NULL) {
      fprintf(stderr, "Error trying to malloc for a RuleNode\n");
      exit(EXIT_FAILURE);
   } else {
      node->name = name;
      node->dependencies = dependencies;
      node->commands = commands;
      node->next = NULL;
   }
   return node;
}

Nodelist *initializeNodelist() {
   Nodelist *list = malloc(sizeof(Nodelist));
   if (list == NULL) {
      fprintf(stderr, "Error tring to malloc for a Nodelist\n");
      exit(EXIT_FAILURE);
   } else {
      list->head = NULL;
   }
   return list;
}

Rulelist *initializeRulelist() {
   Rulelist *list = malloc(sizeof(Rulelist));
   if (list == NULL) {
      fprintf(stderr, "Error trying to malloc for a Rulelist\n");
      exit(EXIT_FAILURE);
   } else {
      list->head = NULL;
   }
   return list;
}

Nodelist *copyNodelist(Nodelist *master_list) {
   Nodelist *copied_list = initializeNodelist();
   CharNode *node = master_list->head;
   while (node != NULL) {
      CharNode *new_node = initializeCharNode(node->data);
      addToNodelist(copied_list, new_node);
      node = node->next;
   }
   return copied_list;
}

void freeNodelist(Nodelist *list) {
   if (list != NULL) {
      CharNode *head = list->head;
      while (head != NULL) {
         CharNode *temp = head;
         head = head->next;
         free(temp);
      }
   }
}

void freeNodelistAndData(Nodelist *list) {
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

void freeRulelist(Rulelist *list) {
   if (list != NULL) {
      RuleNode *head = list->head;
      while (head != NULL) {
         RuleNode *temp = head;
         head = head->next;
         freeNodelist(temp->dependencies);
         free(temp->dependencies);
         freeNodelist(temp->commands);
         free(temp->commands);
         free(temp);
      }
   }
}

void freeAllData(Rulelist *rulelist, Nodelist *master_list, 
                  Nodelist *copied_list) {
   freeRulelist(rulelist);
   free(rulelist);
   freeNodelistAndData(master_list);
   free(master_list);
   freeNodelist(copied_list);
   free(copied_list);
}
