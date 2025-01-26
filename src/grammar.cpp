#include "grammar.h"

#include "ast.h"
#include "production.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include <cstring>

static int debug = true;

// given a string representation of a production rule,  and either
// add to an existing nonterminal, or create a new nonterminal and add
// to the grammar
// part 1 parses the string into a vector of token names
// part 2 processes the tokens into nonterminal and productions
TokenDefList Grammar::parse_rule(const char *rule_name,
				 char *production)
{
  #define PARSE_RULE_INDENT "    "
  #define PARSE_TOKEN_INDENT "      "
  TokenDefList rule_tokens;
  if (!production || ! rule_name) return rule_tokens;
  
  char *scan = production;

  do
  {
    // Extract the next token from the rule string
    while (*scan != '<' && *scan != '"') ++scan;
    
    char start_token = *scan;
    scan++; // now points at start of wrapped token
    bool nonterminal = (start_token == '<');
    char end_token = (nonterminal ? '>' : '"');
    char *scan_end = strchr(scan, end_token);
    
    if (!scan_end)
    {
      return rule_tokens; // invalid
    }
    
    const char *token_str = strndup(scan, (scan_end - scan));

    // Push this token to the list of rule tokens
    printf(PARSE_TOKEN_INDENT "parse token %s \"%s\"\n",
	   (nonterminal ? "non-terminal" : "terminal"),
	   token_str);

    rule_tokens.push_back(TokenParseDef{token_str, nonterminal});
    
    scan = scan_end + 1;
  } while (*scan);

  // Debug printing post-processing of - a rule
  printf(PARSE_RULE_INDENT "parse_rule, printing output:\n");
  for (auto &rt : rule_tokens)
  {
    printf(PARSE_RULE_INDENT "rule token: %s %d\n", rt.name, rt.nonterminal);
  }

  return rule_tokens;
}

// process a line from the language definition file
// will have a lhs (of ":") naming the token and a rhs
// that is either a terminal "name" or a list of productions
// separated by "|" characters.
LineParseDef Grammar::parse_line(char *c_str)
{
  #define PARSE_LINE_INDENT "  "
  LineParseDef prod_def;
  std::vector<TokenDefList> &rule_defs = prod_def.rule_list;
  
  char *lhs = strtok(c_str, ":");
  prod_def.name = lhs;
  char *rhs = strtok(NULL, ":");
  if (!rhs) return prod_def; // bad line

  if (debug)
  {
    std::cout << PARSE_LINE_INDENT "parse name: " << lhs
	      << ", and def: " << rhs << std::endl;
  }

  // Otherwise process production rules
  char *production_string = strtok(rhs, "|");
  std::vector<char*> rule_strings;

  do
  {
    if (debug)
    {
      std::cout << PARSE_RULE_INDENT "parse rule: "
		<< production_string << std::endl;
    }

    rule_strings.push_back(strdup(production_string));
  } while ((production_string = strtok(NULL, "|")));

  for (auto &s : rule_strings)
  {
    TokenDefList ret = parse_rule(lhs, s);
    printf(PARSE_LINE_INDENT "printing output of parse_rule from parse_line:\n");

    for (auto &r : ret)
    {
      printf(PARSE_LINE_INDENT "rt: %s, %d\n", r.name, r.nonterminal);
    }
    printf("\n");
    
    rule_defs.push_back(ret);
  }

  printf(PARSE_LINE_INDENT "printing rule_defs output from parse_line:\n");
  for (TokenDefList &rd : rule_defs)
  {
    printf(PARSE_LINE_INDENT "rd -\n");
    for (TokenParseDef &tpd : rd)
    {
      printf(PARSE_LINE_INDENT "tpd: %s, %d\n", tpd.name, tpd.nonterminal);
    }
  }

  // printf(PARSE_LINE_INDENT "parse_line returning prod_def %p with rule_def %p size %zu\n",
  // 	 &prod_def,
  // 	 &(prod_def.rule_list),
  // 	 prod_def.rule_list.size());
  
  return prod_def;
}

void Grammar::process_production_definition(LineParseDef def)
{
  const char *prod_name = def.name;
  std::vector<TokenDefList> list_of_rules = def.rule_list;

  printf("**** Processing the production definition for %s\n", prod_name);
  
  // new terminal tokens have one rule, with a single terminal token
  if (list_of_rules.size() == 1
      && list_of_rules[0].size() == 1
      && list_of_rules[0][0].nonterminal == false)
  {
    printf("This is a new terminal token\n");
    // add a terminal
    Terminal *new_terminal = new Terminal(list_of_rules[0][0].name);
    addToken(new_terminal);
    return;
  }

  // otherwise this is a nonterminal token
  printf("This is a new NON-terminal token\n");
  
  // Use the rule_tokens to generate a non-terminal and add it to the grammar
  NonTerminal *new_nonterminal = new NonTerminal(prod_name);
  addToken(new_nonterminal);
  printf("Nonterminal %s created %p\n", prod_name, new_nonterminal);

  for (TokenDefList &rule : list_of_rules)
  {
    Production *new_production = new Production(prod_name);

    printf("New production for %s\n", prod_name);
  
    for (TokenParseDef tok : rule)
    {
      const char *add_token = tok.name;
      bool add_nonterminal = tok.nonterminal;

      Token* t = NULL;

      if (!add_nonterminal)
      {
	printf("Adding terminal token %s\n", add_token);
	t = new Terminal(add_token); // duplicates okay for now
      }
      else
      {
	printf("Adding non-terminal token %s\n", add_token);
	
	t = findToken(add_token);
	if (t)
	{
	  printf("non-terminal token %s exists: %p\n", add_token, t);
	}
	else
	{
	  t = new NonTerminal(add_token);
	  printf("new non-terminal token %s created: %p\n", add_token, t);
	}
      }

      new_production->addToken(t);
      new_production->print();
      printf("\n");
    }

    // Does order matter? Wouldn't if we were using pointers...
    new_nonterminal->addProduction(new_production);
  }

  printf(">>>>Printing grammar defintion partway\n");
  print();
  printf("<<<<");

  printf("****\n\n");
}

bool Grammar::load_file(const char *definition_filename)
{
  std::string line;
  std::ifstream in;

  if (debug)
  {
    std::cout << "load_file" << std::endl;
  }
  
  in.open(definition_filename, std::ios::in);
  
  if (!in.is_open())
  {
    std::cout << "Failed to open file " << definition_filename
	      << " for reading." << std::endl;
    return false;
  }

  while(getline(in, line))
  {
    std::cout << "Line: " << line << std::endl;

    char *c_str = strdup(line.c_str());
    LineParseDef prod_def = parse_line(c_str);

    // printf("parse_line returning prod_def %p with rule_def %p size %zu\n",
    // 	   &prod_def,
    // 	   &(prod_def.rule_list),
    // 	   prod_def.rule_list.size());
    
    printf("printing prod_def rule_defs output from load_file:\n");
    
    for (TokenDefList &rd : prod_def.rule_list)
    {
      printf( "rd -\n");
      for (TokenParseDef &tpd : rd)
      {
	printf( "tpd: %s, %d\n", tpd.name, tpd.nonterminal);
      }
    }
    
    process_production_definition(prod_def);
  }

  in.close();
  return true;
}

Grammar::Grammar(const char *definition_filename)
{
  std::cout << "Grammar constructor" << std::endl << std::endl;
  
  load_file(definition_filename);
  
  std::cout << std::endl;
}

void Grammar::addToken(Token *t) { definition.push_back(t); }

Token* Grammar::findToken(const char *token_name)
{
  const auto it =
    std::find_if(definition.begin(), definition.end(),
		 [token_name](Token* t) {
		   return (t->name() == token_name);
		 });

  if (it == definition.end())
  {
    return NULL;
  }

  return *it;
}

void Grammar::parseSource(input_stream source)
{
  std::cout << "parseSource" << std::endl;

  for (auto &s : source) std::cout << s << " ";
  std::cout << std::endl;

  // Find a token
  std::vector<TokenTreeNode*> children;
  int incr = 0; // input-token offset

  while (incr < (source.size() - 1))
  {
    int depth = 0;
    bool match_any = false;
    TokenTreeNode *match = NULL;
    
    for (auto &t : definition)
    {
      // Skip any definitions that are too long
      bool did_match = t->match(&source[incr],
				source.size() - incr,
				incr,
				depth,
				match);

      if (did_match)
      {
        std::cout << "Matched token \"" << t->name();
        std::cout << "\" (toks=" << incr << ")" << std::endl;
        
        match_any = true;
	break;
      }
    }

    if (!match_any)
    {
      std::cout << "Syntax error - grammar did not match any tokens" << std::endl;
      exit(-1);
    }

    children.push_back(match);
  }

  std::cout << "parse loop end" << std::endl;

  // TODO: Move this logic
  for (auto &c : children)
  {
    std::cout << "print a child" << std::endl;
    printTokenTree(*c);
  }
}

void Grammar::print()
{
  printf("Grammar definition:\n");
  
  for (auto &t : definition)
  {
    t->print();
    printf("\n");
  }

  printf("\n");
}

Grammar& operator<<(Grammar &g, Token &t)
{
  g.addToken(&t);
  return g;
}
