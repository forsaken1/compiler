#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

using namespace std;

class Token {
	string type, text;
	int pos, line, valueInt;
	float valueFloat;

public:
	Token(int, int, string, string, string*);
	
	string GetType();
	string GetText();
	int GetPos();
	int GetLine();
	int GetValueInt();
	float GetValueFloat();
	void Print();
};

#endif 
