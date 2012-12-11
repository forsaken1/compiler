#include "token.h"

Token::Token(int _line, int _pos, TokenType _t, string _type, string _text) {
	pos = _pos;
	line = _line;
	type = _type;
	text = _text;
	t = _t;
}

void Token::Print(ofstream& outputStream) {
	outputStream << line << '\t' << pos << '\t' << type << "\t\t" << text << endl;
}

TokenType Token::GetType() {
	return t;
}

string Token::GetTypeName() {
	return type;
}

string Token::GetText() {
	return text;
}

int Token::GetPos() {
	return pos;
}

int Token::GetLine() {
	return line;
}

//---Integer---
TokenInteger::TokenInteger(int _line, int _pos, string _type, string _text, int _value): Token(_line, _pos, _type, _text) {
	value = _value;
}

int TokenInteger::GetValue() {
	return value;
}

//---Real---
TokenReal::TokenReal(int _line, int _pos, string _type, string _text, double _value): Token(_line, _pos, _type, _text) {
	value = _value;
}

double TokenReal::GetValue() {
	return value;
}

//---String---
TokenString::TokenString(int _line, int _pos, string _type, string _text, string _value): Token(_line, _pos, _type, _text) {
	value = _value;
}

string TokenString::GetValue() {
	return value;
}
