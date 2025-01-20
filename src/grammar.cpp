#include "grammar.h"

#include "ast.h"
#include "production.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include <cstring>

static int debug = false;

// technically this can just be a token definition
void Grammar::process_rule(const char *rule_name, char *production)
{
  // Decode production
  // "a" or <a><b> etc
  char *scan = production;
  const char *end_char = NULL;

  // TODO: Seems inefficient, maybe refactor classes?
  Production *new_production = NULL;
  NonTerminal *new_nonterminal = NULL;
  
  do {
    // scan until we find either < or "
    while (   (*scan) != '"'
	      && (*scan) != '<' ) ++scan;

    bool is_token = ((*scan) == '"');
    end_char = (is_token ? "\"" : ">");
    ++scan;

    //  then get the corresponding 'end' character
    char *end_pos = strchr(scan, end_char[0]);
    if (!end_pos)
    {
      printf("Couldn't find next \"%c\"\n", end_char[0]);
      return;
    }

    // print the production definition token
    char *token_name = strndup(scan, (end_pos - scan));
    
    if (is_token)
    {
      // just add a token to the list
      if (debug)
      {
	printf("Terminal token: %s\n", token_name);
      }
      
      Token *new_token = new Terminal(token_name);
      (*this) << (*new_token);
      
      // we only allow one new terminal definiton on a rule
      return;
    }
    else
    {
      if (debug)
      {
	printf("Production rule token: %s\n", token_name);
      }

      if (!new_nonterminal)
      {
	new_nonterminal = new NonTerminal(rule_name);
      }

      if (!new_production)
      {
	new_production = new Production(rule_name);
      }
      
      // Need to look up an existing token in the grammar

      // Check if this is the rule name, we will use a placeholder..
      if (0 == strcmp(token_name, rule_name))
      {
	(*new_production) << (*new_nonterminal);
      }
      else
      {
	Token* t = findToken(token_name);
	if (t) (*new_production) << (*t);
      }
    }
    
    scan = end_pos + 1;
  } while (*scan);

  // Create a new rule with the children and the token name
  (*new_nonterminal) << (*new_production);
  (*this) << (*new_nonterminal);
}

void Grammar::process_line(char *c_str)
{
  // triplet_token:<a>|<b>|<c>
  const char *token_name = strtok(c_str, ":");

  if (!token_name)
  {
    std::cout << "Missing colon, skipping line" << std::endl;
    return;
  }

  if (debug)
  {
    std::cout << "  Token name: " << token_name << std::endl;
  }
    
  char *rhs = NULL;
  int token_count = 0;

  while ((rhs = strtok(NULL, "|")))
  {
    ++token_count;

    if (debug)
    {
      std::cout << "    got production: " << rhs << std::endl;
    }

    process_rule(token_name, rhs);
  } 

  if (debug)
  {
    printf("Got %d tokens\n", token_count);
  }
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

    // Tokenise on ":" for token name and then "|" for definition tokens
    char *c_str = strdup(line.c_str());
    process_line(c_str);
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

Token* Grammar::findToken(const char* token_name)
{
  const auto it = std::find_if(definition.begin(), definition.end(),
			       [token_name](Token* t) {
				 return (0 == strcmp(t->name().c_str(),token_name));
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
