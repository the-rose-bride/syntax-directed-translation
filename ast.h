#ifndef AST_H
#define AST_H

#include "token.h"

#include <vector>

class TokenTreeNode
{
 private:
  Token *token;
  std::vector<TokenTreeNode*> children;
  
 public:
  TokenTreeNode(Token *token);
  TokenTreeNode(Token *token, std::vector<TokenTreeNode*> children);

  friend void printTokenTree(TokenTreeNode &node);
};

#endif // AST_H
