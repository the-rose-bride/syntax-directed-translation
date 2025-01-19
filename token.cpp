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

bool Terminal::match(std::string *token_stream,
		     int num_tokens,
		     int &incr,
		     int depth,
		     TokenTreeNode *&match_tree)
{
  if (debug)
  {
    indent_n(depth);
    std::cout << "Terminal " << m_definition
              << " match on " << token_stream[0] << std::endl;
  }

  bool match = (token_stream[0] == m_definition);

  if (match)
  {
    incr++;
    match_tree = new TokenTreeNode(this);
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

bool NonTerminal::match(std::string *token_stream,
			int num_tokens,
			int &incr,
			int depth,
			TokenTreeNode *&match_tree)
{
  if (debug)
  {
    indent_n(depth);
    std::cout << "Nonterminal " << m_definition
              << " match on " << token_stream[0] << std::endl;
  }
  
  // Iterate over productions and match on them
  for (auto& p : productions)
  {
    if (p->numTokens() > num_tokens) continue;
    
    int prod_incr = 0;

    if (p->match(&token_stream[0],
		 num_tokens,
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
