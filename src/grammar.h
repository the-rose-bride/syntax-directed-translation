#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>

#include "token.h"

typedef std::vector<std::string> input_stream;

struct TokenParseDef
{
  const char *name;
  bool nonterminal;
};

typedef std::vector<TokenParseDef> TokenDefList;

struct LineParseDef
{
  const char *name;
  std::vector<TokenDefList> rule_list;
};

class Grammar
{
private:
  std::vector<Token*> definition;

  TokenDefList parse_rule(const char *rule_name,
			  char *production);
  LineParseDef parse_line(char *c_str);
  void process_production_definition(LineParseDef prod_def);
  bool load_file(const char *definition_filename);
  
public:
  Grammar(const char *definition_filename);
  void addToken(Token *t);
  Token* findToken(const char *token_name);
  void parseSource(input_stream source);
  void print();
  
  friend Grammar& operator<<(Grammar &g, Token &t);
};

#endif // GRAMMAR_H
