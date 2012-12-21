#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

using namespace std;

enum TokenType {
	KEYWORD,
	IDENTIFIER,
	CONST_REAL,
	CONST_INTEGER,
	CONST_CHAR,
	CONST_STRING,
	ESCAPE_SEQUENCE,
	SEPARATOR,
	OPERATOR
};

class Token {
protected:
	string type, text;
	int pos, line;
	TokenType t;

public:
	Token(int, int, TokenType, string, string);
	
	TokenType GetType();
	string GetTypeName();
	string GetText();
	int GetPos();
	int GetLine();
	virtual void Print();
};

//---Integer---
class TokenInteger: public Token {
	int value;

public:
	TokenInteger(int, int, TokenType, string, string);
	
	int GetValue();
};

//---Real---
class TokenReal: public Token {
	double value;

public:
	TokenReal(int, int, TokenType, string, string);

	double GetValue();
};

//---String---
class TokenString: public Token {
	string value;

public:
	TokenString(int, int, TokenType, string, string);
	
	string GetValue();
};

#endif TOKEN_H
