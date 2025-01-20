#include "ast.h"

#include "helper.h"

#include <iostream>

// S -> a S b S
//
// this node will have four children
// the two S token children will also have children

TokenTreeNode::TokenTreeNode(Token *token) : token(token) {}

TokenTreeNode::TokenTreeNode(Token *token,
                             std::vector<TokenTreeNode *> children)
    : token(token), children(children) {}

// friend function
void printTokenTree(TokenTreeNode &node)
{
  static int depth = 0;

  indent_n(depth);

  depth++;
  
  std::cout << "(T)" << (*node.token) << std::endl;
  
  for (auto &c : node.children)
  {
    printTokenTree(*c);
  }

  depth--;
}
