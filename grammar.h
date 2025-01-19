#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>

#include "token.h"

typedef std::vector<std::string> input_stream;

class Grammar
{
private:
  std::vector<Token*> definition;
  
public:
  void addToken(Token *t);
  void parseSource(input_stream source);
  void print();
  
  friend Grammar& operator<<(Grammar &g, Token &t);
};

#endif // GRAMMAR_H
