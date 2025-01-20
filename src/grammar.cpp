#include "grammar.h"

#include "ast.h"

#include <iostream>

void Grammar::addToken(Token *t)
{
  definition.push_back(t);
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
