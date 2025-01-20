#include "grammar.h"

#include "ast.h"
#include "production.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include <cstring>

static int debug = true;

// given a token name, add a new token to the grammar
void Grammar::process_token(const char *token_name)
{
  // just add a token to the list
  if (debug)
  {
    printf("Terminal token: %s\n", token_name);
  }
      
  Token *new_token = new Terminal(token_name);
  (*this) << (*new_token);
      
  return;
}

// given a string representation of a production rule, parse and either
// add to an existing nonterminal, or create a new nonterminal and add
// to the grammar
// part 1 parses the string into a vector of token names
// part 2 processes the tokens into nonterminal and productions
void Grammar::process_rule(const char *rule_name, NonTerminal *nonterminal, char *production)
{
  // Decode non-terminal production
  // <a><b> etc
  char *token = strtok(production, ">");
  std::vector<const char*> rule_tokens;
  
  do
  {
    if (*token == '<') token++;
    
    // print the production definition token
    char *token_name = strdup(token);

    if (debug)
    {
      printf("Production rule token: %s\n", token_name);
    }

    rule_tokens.push_back(token_name);
  } while ((token = strtok(NULL, ">")));

  // Use the rule_tokens to generate a non-terminal and add it to the grammar
  // TODO: Seems inefficient, maybe refactor classes?
  Production *new_production = new Production(rule_name);
  
  for (auto &str : rule_tokens)
  {
    // Need to look up an existing token in the grammar
    // Check if this is the rule name, we will use a placeholder..
    if (0 == strcmp(str, rule_name))
    {
      (*new_production) << (*nonterminal);
    }
    else
    {
      std::string token_string = std::string(str);
      Token* t = findToken(token_string);
      if (t) (*new_production) << (*t);
    }
  }
  
  // Create a new rule with the children and the token name
  (*nonterminal) << (*new_production);
}

// process a line from the language definition file
// will have a lhs (of ":") naming the token and a rhs
// that is either a terminal "name" or a list of productions
// separated by "|" characters.
void Grammar::process_line(char *c_str)
{
  char *lhs = strtok(c_str, ":");
  char *rhs = strtok(NULL, ":");
  if (!rhs) return; // bad line

  if (debug)
  {
    std::cout << "  Token name: " << lhs
	      << ", definition: " << rhs << std::endl;
  }

  // First check we are not a new terminal token definition
  const char *open_quote = strchr(rhs, '"');
  if (open_quote)
  {
    const char *close_quote = strchr(open_quote + 1, '"');
    if (!close_quote)
    {
      printf("missing close quote from line rhs\n");
      return;
    }
    open_quote++;
    const char *token = strndup(open_quote, (close_quote - open_quote));
    process_token(token);
    return;
  }

  // Otherwise process production rules
  char *production_string = strtok(rhs, "|");
  std::vector<char*> rule_strings;

  do
  {
    if (debug)
    {
      std::cout << "    got production: " << production_string << std::endl;
    }

    rule_strings.push_back(production_string);
  } while ((production_string = strtok(NULL, "|")));

  // process rule calls strtok, we can't call until strtok is finished above
  NonTerminal *new_nonterminal = new NonTerminal(lhs);
  addToken(new_nonterminal);
  
  for (auto &s : rule_strings)
  {
    process_rule(lhs, new_nonterminal, s);
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

Token* Grammar::findToken(std::string &token_name)
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
