#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

using namespace std;

enum TokenType {
	END_OF_FILE,
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

enum TokenValue {
	DEFAULT,
	//--- SEPARATORS ---
	ROUND_LEFT_BRACKET,
	ROUND_RIGHT_BRACKET,
	SQUARE_RIGHT_BRACKET,
	SQUARE_LEFT_BRACKET,
	FIGURE_RIGHT_BRACKET,
	FIGURE_LEFT_BRACKET,
	SEMICOLON,
	COMMA,
	//--- OPERATIONS ---
	OPER_ASSIGN,
	OPER_EQUAL,
	OPER_PLUS,
	OPER_MINUS,
	OPER_MULTIPLY,
	OPER_DIVIDE,
	OPER_DIVIDE_BY_MOD,
	OPER_NOT,
	OPER_NOT_EQUAL,
	OPER_MORE,
	OPER_LESS,
	OPER_MORE_OR_EQUAL,
	OPER_LESS_OR_EQUAL,
	OPER_AND,
	OPER_OR,
	OPER_BINARY_AND,
	OPER_BINARY_OR,
	OPER_BINARY_NOT,
	OPER_INC,
	OPER_DEC,
	OPER_PLUS_EQUAL,
	OPER_MINUS_EQUAL,
	OPER_MULTIPLY_EQUAL,
	OPER_DIVIDE_EQUAL,
	OPER_DIVIDE_BY_MOD_EQUAL,
	OPER_BINARY_AND_EQUAL,
	OPER_BINARY_OR_EQUAL,
	OPER_ARROW,
	OPER_QUESTION,
	OPER_COLON,
	OPER_POINT,
	OPER_SHIFT_LEFT,
	OPER_SHIFT_RIGHT,
	OPER_EXCLUSIVE_OR,
	OPER_EXCLUSIVE_OR_EQUAL,
	//--- KEYWORDS ---
	KEYWORD_BREAK,
	KEYWORD_CHAR,
	KEYWORD_CONTINUE,
	KEYWORD_DO,
	KEYWORD_ELSE,
	KEYWORD_FLOAT,
	KEYWORD_FOR,
	KEYWORD_GOTO,
	KEYWORD_IF,
	KEYWORD_INT,
	KEYWORD_RETURN,
	KEYWORD_SIZEOF,
	KEYWORD_STRUCT,
	KEYWORD_VOID,
	KEYWORD_WHILE,
	KEYWORD_PRINT
};

class Token {
protected:
	string type, text;
	int pos, line;
	TokenType t;
	TokenValue v;

public:
	Token(int _line, int _pos, TokenType _t, TokenValue _v, string _text) {
		pos = _pos;
		line = _line;
		text = _text;
		t = _t;
		v = _v;
		type = GetTokenTypeString(t);
	}

	string GetTokenTypeString(TokenType _t) {
		switch(_t) {
			case KEYWORD:			return "KEYWORD";
			case IDENTIFIER:		return "IDENTIF";
			case CONST_REAL:		return "REAL";
			case CONST_INTEGER:		return "INT";
			case CONST_CHAR:		return "CHAR";
			case CONST_STRING:		return "STRING";
			case ESCAPE_SEQUENCE:	return "ESCAPE";
			case SEPARATOR:			return "SEPARAT";
			case OPERATOR:			return "OPERAT";
			case END_OF_FILE:		return "EOF";
		}
	}
	
	TokenValue GetTokenValue() {
		return v;
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
