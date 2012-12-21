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
	OPERATOR,
	END_OF_FILE
};

class Token {
protected:
	string type, text;
	int pos, line;
	TokenType t;

public:
	Token(int _line, int _pos, TokenType _t, string _type, string _text) {
		pos = _pos;
		line = _line;
		type = _type;
		text = _text;
		t = _t;
	}
	
	TokenType GetType() {
		return t;
	}

	string GetTypeName() {
		return type;
	}

	string GetText() {
		return text;
	}

	int GetPos() {
		return pos;
	}

	int GetLine() {
		return line;
	}

	virtual void Print() {
		cout << line << '\t' << pos << '\t' << type << "\t\t" << text << endl;
	}
};

#endif TOKEN_H
