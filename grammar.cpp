#include "grammar.h"

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

  while (*s)
  {
    int incr = 0;
    int depth = 0;

    bool match_any = false;
    
    for (auto &t : definition)
    {
      bool did_match = t->match(s, incr, depth); // will need to pass a tree in

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
  }

  std::cout << "loop end" << std::endl;
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
