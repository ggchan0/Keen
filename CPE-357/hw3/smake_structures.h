#ifndef SMAKE_STRUCTURES_H
#define SMAKE_STRUCTURES_H

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

void addToNodelist(Nodelist *nodelist, CharNode *charNode);

void addToRulelist(Rulelist *rulelist, RuleNode *ruleNode);

void printNodelist(Nodelist *list);

void printRuleNode(RuleNode *rule);

void printRulelist(Rulelist *list);

int noTarget(RuleNode *node);

CharNode *initializeCharNode(char *data);

RuleNode *initializeRuleNode(char *name,
                              Nodelist *dependencies, Nodelist *commands);
      
Nodelist *initializeNodelist();

Rulelist *initializeRulelist();

Nodelist *copyNodelist(Nodelist *master_list);

void freeNodelist(Nodelist *list);

void freeNodelistAndData(Nodelist *list);

void freeRulelist(Rulelist *list);

void freeAllData(Rulelist *rulelist, Nodelist *master_list, 
                  Nodelist *copied_list);

#endif