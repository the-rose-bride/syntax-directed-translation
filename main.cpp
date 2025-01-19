#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "token.h"
#include "production.h"
#include "grammar.h"

int main(int argc, char **argv)
{
  printf("Hello, SYNTAX DIRECTED TRANSLATOR\n\n");

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
  
  NonTerminal triplet_token("triplet_token");
  
  Production P_triplet_token_A("P_triplet_token_A");
  P_triplet_token_A << a;
  Production P_triplet_token_B("P_triplet_token_B");
  P_triplet_token_B << b;
  Production P_triplet_token_C("P_triplet_token_C");
  P_triplet_token_C << c;

  triplet_token << P_triplet_token_A
		<< P_triplet_token_B
		<< P_triplet_token_C;

  NonTerminal triplet("triplet");
  Production P_triplet("P_triplet");
  P_triplet << triplet_token << triplet_token << triplet_token;
  triplet << P_triplet;

  Grammar g;
  g << S << triplet << triplet_token;
  g.print();
  
  // Read in a language definition
  // S>
  // aSbS
  // bSaS
  // c
  // Should achieve the above code

  // Read in some 'source code'
  const char *source = "a c b b c a c";
  if (argc > 1) source = argv[1];
  
  // Parse and represent the syntax tree
  g.parseSource(source);

  // print tree here
  
  
  return 0;
}
