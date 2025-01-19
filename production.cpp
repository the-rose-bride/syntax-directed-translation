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

int Production::numTokens()
{
  return definition.size();
}  

void Production::print()
{
  //std::cout << m_name << ": ";
  
  for (auto &t : definition)
  {
    std::cout << t->name() << " ";
  }
}

bool Production::match(std::string *token_stream,
		       int num_tokens,                       
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

  if (num_tokens < definition.size()) return false;
  
  // Iterate over tokens and match on each one
  int sub_incr = 0;
  std::vector<TokenTreeNode*> children;
  
  for (auto& t : definition)
  {
    TokenTreeNode *sub_match_tree = NULL;
    
    if (!t->match(&token_stream[sub_incr],
		  num_tokens - sub_incr,
                  sub_incr,
                  depth + 1,
                  sub_match_tree))
    {
      return false;
    }
    
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
