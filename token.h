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
	virtual void Print(ofstream&);
};

//---Integer---
class TokenInteger: public Token {
	int value;

public:
	TokenInteger(int, int, string, string, int);
	
	int GetValue();
};

//---Real---
class TokenReal: public Token {
	double value;

public:
	TokenReal(int, int, string, string, double);

	double GetValue();
};

//---String---
class TokenString: public Token {
	string value;

public:
	TokenString(int, int, string, string, string);
	
	string GetValue();
};

#endif 
