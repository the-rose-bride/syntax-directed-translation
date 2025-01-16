#include "production.h"

#include <iostream>
#include <algorithm>

#include "token.h"
#include "helper.h"

Production::Production(std::string name)
{
  m_name = name;
}

void Production::addToken(Token *token) { definition.push_back(token); }

void Production::print()
{
  std::cout << m_name << ": ";
  
  for (auto &t : definition)
  {
    t->print();
    std::cout << " ";
  }
}

bool Production::match(const char *str, int &incr, int depth)
{
  indent_n(depth);
  print();
  std::cout << " match " << std::endl;
  
  // Iterate over tokens and match on each one
  int sub_incr = 0;
  
  for (auto& t : definition)
  {
    while (isspace(*(str + sub_incr)))
    {
      ++sub_incr;
    }
    
    if (!t->match(str + sub_incr, sub_incr, depth + 1)) return false;
  }

  // Here if the whole production matches
  incr += sub_incr;
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
