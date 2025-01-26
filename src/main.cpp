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

  // Read in a language definition here
  Grammar g("./def.txt");
  g.print();

  // Read in some 'source code'
  const char *source = "a c b b c a c";
  if (argc > 1) source = argv[1];

  // Tokenise input
  std::cout << "Tokenising input.." << std::endl;
  std::vector<std::string> input = tokenise_input(source);

  // Parse and represent the syntax tree
  g.parseSource(input);

  // print tree here
  // TODO: currently done in parseSource()
  
  return 0;
}
