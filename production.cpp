#include "production.h"

#include <iostream>
#include <algorithm>

#include "token.h"
#include "helper.h"

static bool debug = true;

Production::Production(std::string name)
{
  m_name = name;
}

void Production::setParent(NonTerminal *parent)
{
  this->parent = parent;
}

void Production::addToken(Token *token) { definition.push_back(token); }

void Production::print()
{
  //std::cout << m_name << ": ";
  
  for (auto &t : definition)
  {
    std::cout << t->name() << " ";
  }
}

bool Production::match(const char *str,
		       int &incr,
		       int depth,
		       TokenTreeNode* &match_tree)
{
  if (debug)
  {
    indent_n(depth);
    print();
    std::cout << " match " << std::endl;
  }
  
  // Iterate over tokens and match on each one
  int sub_incr = 0;
  std::vector<TokenTreeNode*> children;
  
  for (auto& t : definition)
  {
    // TODO: tokenise your input, you animal
    while (isspace(*(str + sub_incr))) ++sub_incr;

    TokenTreeNode *sub_match_tree = NULL;
    
    if (!t->match(str + sub_incr,
                  sub_incr,
                  depth + 1,
                  sub_match_tree))
    {
      return false;
    }

    // printTokenTree(*sub_match_tree);
    
    children.push_back(sub_match_tree);
  }

  // Here if the whole production matches
  incr += sub_incr;
  match_tree = new TokenTreeNode(parent, children);
  
  if (debug)
  {
    indent_n(depth);
    std::cout << "production tree being returned" << std::endl;
  }
  
  return true;
}

std::ostream& operator<<(std::ostream& stream, const Production &p)
{
  std::for_each(p.definition.begin(), p.definition.end(),
                [&stream](Token *t) {
                  stream << t;
                });
  return stream;
}

Production &operator<<(Production &p, Token &t)
{
  p.addToken(&t);
  return p;
}
