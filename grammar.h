#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>

#include "token.h"

class Grammar
{
private:
  std::vector<Token*> definition;
public:
  void addToken(Token *t);
  void parseSource(const char *source);
  void print();
  
  friend Grammar& operator<<(Grammar &g, Token &t);
};

#endif // GRAMMAR_H
