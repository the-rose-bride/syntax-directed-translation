#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

class TokenTreeNode;

class Production;

// Parent class
class Token
{
protected:
  std::string m_definition;
  
public:
  Token(std::string definition);

  std::string name();
  
  virtual void print() = 0;
  virtual bool match(std::string *token_stream,
		     int num_tokens,
		     int &incr,
		     int depth,
		     TokenTreeNode* &match_tree) = 0;
  
  friend std::ostream& operator<<(std::ostream& stream, const Token &token);
  
};

// Child classes
class Terminal : public Token
{
public:
  Terminal(std::string definition);

  void print();
  bool match(std::string *token_stream,
	     int num_tokens,
	     int &incr,
	     int depth,
	     TokenTreeNode *&match_tree);
};

class NonTerminal : public Token
{
private:
  std::vector<Production*> productions;
  
public:
  NonTerminal(std::string definition);
  
  void addProduction(Production *production);
  void printProductions();
  void print();
  bool match(std::string *token_stream,
	     int num_tokens,
	     int &incr,
	     int depth,
	     TokenTreeNode *&match_tree);
    
  friend NonTerminal& operator<<(NonTerminal &nt, Production &p);
};


#endif
