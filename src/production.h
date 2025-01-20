#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <iostream>
#include <vector>

#include "ast.h"

class Token;
class NonTerminal;

class Production
{
private:
  std::string m_name;
  std::vector<Token*> definition;
  NonTerminal *parent; // what nonterminal token do we belong to?
  
public:
  Production(std::string name);
  void setParent(NonTerminal *parent);
  void addToken(Token *token);
  int numTokens();
  void print();
  bool match(std::string *token_stream,
	     int num_tokens,
	     int &incr,
	     int depth,
	     TokenTreeNode* &match_tree);
  
  friend std::ostream& operator<<(std::ostream& stream, const Production &p);
  friend Production& operator<<(Production &p, Token &t);
};

#endif
