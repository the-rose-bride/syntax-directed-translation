#include "token.h"

#include <iostream>
#include <algorithm>

#include <cstring>

#include "production.h"
#include "helper.h"

Token::Token(std::string definition) : m_definition(definition) {}

void Token::print() { std::cout << m_definition; }

std::ostream& operator<<(std::ostream& stream, const Token &token)
{
  stream << token.m_definition;
  return stream;
}

// Terminals
Terminal::Terminal(std::string definition) : Token(definition) {}

bool Terminal::match(const char *str, int &incr, int depth)
{
  indent_n(depth);
  
  std::cout << "Terminal " << m_definition
            << " match on " << str << std::endl;

  bool match = (0 == strncmp(str, m_definition.c_str(), m_definition.size()));

  if (match) incr++; // inrement by length
  
  return match;
}

// Non-terminals
NonTerminal::NonTerminal(std::string definition) : Token(definition) {}

void NonTerminal::addProduction(Production *production)
{
  productions.push_back(production);
}

void NonTerminal::printProductions()
{
  // print each production on a new line
  std::for_each(productions.begin(), productions.end(),
                [this](Production *p) {
                  print();
                  std::cout << " -> ";
                  p->print();
                  std::cout << std::endl;
                });
}

bool NonTerminal::match(const char *str, int &incr, int depth)
{
  indent_n(depth);
  std::cout << "Nonterminal " << m_definition
            << " match on " << str << std::endl;

  // Skip any whitespace before checking against productions
  while (isspace(*str))
  {
    ++str;
    ++incr;
  }
  
  // Iterate over productions and match on them
  for (auto& p : productions)
  {
    int prod_incr = 0;
    
    if (p->match(str, prod_incr, depth + 1))
    {
      indent_n(depth);
      std::cout << "We matched a production!" << std::endl << std::endl;
      
      incr += prod_incr;
      return true;
    }
  }

  std::cout << std::endl << std::endl;
  
  return false;
}

NonTerminal& operator<<(NonTerminal &nt, Production &p)
{
  nt.addProduction(&p);
  return nt;
}
