#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

class Production;

// Parent class
class Token
{
protected:
  std::string m_definition;
public:
  Token(std::string definition);
  
  void print();
  virtual bool match(const char *str, int &incr, int depth) = 0;
  
  friend std::ostream& operator<<(std::ostream& stream, const Token &token);
  
};

// Child classes
class Terminal : public Token
{
public:
  Terminal(std::string definition);
  
  bool match(const char *str, int &incr, int depth);
};

class NonTerminal : public Token
{
private:
  std::vector<Production*> productions;
  
public:
  NonTerminal(std::string definition);
  
  void addProduction(Production *production);
  void printProductions();
  bool match(const char *str, int &incr, int depth);
    
  friend NonTerminal& operator<<(NonTerminal &nt, Production &p);
};


#endif
