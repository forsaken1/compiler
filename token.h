#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Token {
protected:
	string type, text;
	int pos, line;

public:
	Token(int, int, string, string);
	
	string GetType();
	string GetText();
	int GetPos();
	int GetLine();
	virtual void Print(ostream&);
};

//---Integer---
class TokenInteger: public Token {
	string value;

public:
	TokenInteger(int, int, string, string, string);
	
	string GetValue();
};

//---Real---
class TokenReal: public Token {
	string value;

public:
	TokenReal(int, int, string, string, string);

	string GetValue();
};

//---String---
class TokenString: public Token {
	string value;

public:
	TokenString(int, int, string, string, string);
	
	string GetValue();
};

#endif 
