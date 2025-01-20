#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>

#include "token.h"

typedef std::vector<std::string> input_stream;

class Grammar
{
private:
  std::vector<Token*> definition;
  
  void process_rule(const char *rule_name, char *production);
  void process_line(char *c_str);
  bool load_file(const char *definition_filename);
  
public:
  Grammar(const char *definition_filename);
  void addToken(Token *t);
  Token* findToken(const char* token_name);
  void parseSource(input_stream source);
  void print();
  
  friend Grammar& operator<<(Grammar &g, Token &t);
};

#endif // GRAMMAR_H
