#include "token.h"

#include <iostream>
#include <algorithm>

#include <cstring>

#include "production.h"
#include "helper.h"
#include "ast.h"

static bool debug = true;

Token::Token(std::string definition) : m_definition(definition) {}

std::string Token::name() { return m_definition; }

std::ostream& operator<<(std::ostream& stream, const Token &token)
{
  stream << token.m_definition;
  return stream;
}

// Terminals
Terminal::Terminal(std::string definition) : Token(definition) {}

void Terminal::print()
{
  std::cout << name();
}

bool Terminal::match(const char *str,
		     int &incr,
		     int depth,
		     TokenTreeNode *&match_tree)
{
  if (debug)
  {
    indent_n(depth);
    std::cout << "Terminal " << m_definition
              << " match on " << str << std::endl;
  }

  bool match = (0 == strncmp(str, m_definition.c_str(), m_definition.size()));

  if (match)
  {
    incr++; // increment by length
    match_tree = new TokenTreeNode(this);

    // std::cout << "print terminal tree: ";
    // printTokenTree(*match_tree);
  }
  
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
  for (auto &p : productions)
  {
    std::cout << name() << " -> ";
    p->print();
    std::cout << std::endl;
  };
}

void NonTerminal::print()
{
  std::cout << name() << std::endl;
  printProductions();
}

bool NonTerminal::match(const char *str,
			int &incr,
			int depth,
			TokenTreeNode *&match_tree)
{
  if (debug)
  {
    indent_n(depth);
    std::cout << "Nonterminal " << m_definition
              << " match on " << str << std::endl;
  }

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

    if (p->match(str,
		 prod_incr,
		 depth + 1, 
		 match_tree))
    {
      if (debug)
      {
        indent_n(depth + 1);
        std::cout << "We matched a production!" << std::endl << std::endl;
      }
      
      incr += prod_incr;

      printTokenTree(*match_tree);

      // NOTE: 
      // match tree already represents this nonterminal token,
      // and the matching children
      
      return true;
    }
  }

  std::cout << std::endl << std::endl;
  
  return false;
}

NonTerminal& operator<<(NonTerminal &nt, Production &p)
{
  nt.addProduction(&p);
  p.setParent(&nt);
  return nt;
}
