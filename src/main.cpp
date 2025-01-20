#include <cstdio>
#include <cstring>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "token.h"
#include "production.h"
#include "grammar.h"

input_stream tokenise_input(const char *source_text)
{
  input_stream is;

  char *copy = strdup(source_text);
  
  const char *delim = " ";
  char *tok = strtok(copy, delim);

  while (tok)
  {
    std::cout << tok << std::endl;
    is.push_back(tok);

    tok = strtok(NULL, delim);
  }
  
  return is;
}

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

  // Tokenise input
  std::vector<std::string> input = tokenise_input(source);

  
  // Parse and represent the syntax tree
  g.parseSource(input);

  // print tree here
  
  
  return 0;
}
