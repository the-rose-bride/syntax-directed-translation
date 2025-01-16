#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <iostream>
#include <vector>

class Token;

class Production
{
private:
  std::string m_name;
  std::vector<Token*> definition;
  
public:
  Production(std::string name);
  void addToken(Token *token);
  void print();
  bool match(const char *str, int &incr, int depth);
  
  friend std::ostream& operator<<(std::ostream& stream, const Production &p);
  friend Production& operator<<(Production &p, Token &t);
};

#endif
