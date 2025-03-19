/* tree.h */

#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  char * label;
  char * value;
  struct Node *firstChild, *nextSibling;
  int lineno;
} Node;

Node *makeNode(char * label);
void addSibling(Node *node, Node *sibling);
void addChild(Node *parent, Node *child);
void deleteTree(Node*node);
void printTree(Node *node);

#define FIRSTCHILD(node) node->firstChild
#define SECONDCHILD(node) node->firstChild->nextSibling
#define THIRDCHILD(node) node->firstChild->nextSibling->nextSibling

#endif /* TREE.H */