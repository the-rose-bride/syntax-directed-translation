#include "grammar.h"

#include "ast.h"

#include <iostream>

void Grammar::addToken(Token *t)
{
  definition.push_back(t);
}

void Grammar::parseSource(const char *source)
{
  std::cout << "parseSource" << std::endl;

  std::cout << source << std::endl;

  // Find a token
  const char *s = source;
  std::vector<TokenTreeNode*> children;

  while (*s)
  {
    int incr = 0;
    int depth = 0;

    bool match_any = false;

    TokenTreeNode *match = NULL;
    
    for (auto &t : definition)
    {
      bool did_match = t->match(s, incr, depth, match);

      if (did_match)
      {
        std::cout << "Matched token \"" << t->name();
        std::cout << "\" (chars=" << incr << ")" << std::endl;

        printf("%.*s\n", incr, s);
        
        match_any = true;
        s += incr;
        while (isspace(*s)) ++s;
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
