#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "token.h"
#include "production.h"

// "a c b b c a c"
//
// S -> a S b S | b S a S | c
void parseSource(Token *token, const char *source)
{
  std::cout << "parseSource" << std::endl;

  // Find a token
  const char *s = "a c b c";
  
  int incr = 0;
  int depth = 0;
  
  token->match(s, incr, depth); // will need to pass a tree in
}

int main(int argc, char **argv)
{
  printf("Hello, SDT\n");

  // Represent a language definition - productions, terminals, nonterminals
  Terminal a("a");
  Terminal b("b");
  Terminal c("c");
  
  NonTerminal S("S");
  
  Production P1("P1");
  P1 << a << S << b << S;

  Production P2("P2");
  P2 << b << S << a << S;

  Production P3("P3");
  P3 << c;

  S << P1 << P2 << P3;
  

  S.printProductions();
  std::cout << "print prod fin" << std::endl << std::endl;;
  
  // Read in a language definition
  // S>
  // aSbS
  // bSaS
  // c
  // Should achieve the above code

  // Read in some 'source code'
  const char *source = "a c b b c a c";
  
  // Parse and represent the syntax tree
  parseSource(&S, source);
  
  return 0;
}
