#include "token.h"

Token::Token(int _line, int _pos, string _type, string _text) {
	pos = _pos;
	line = _line;
	type = _type;
	text = _text;
}

void Token::Print() {
	cout << line << '\t' << pos << '\t' << type << "\t\t" << text << "\t" << endl;
}

string Token::GetType() {
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
TokenInteger::TokenInteger(int _line, int _pos, string _type, string _text, string _value): Token(_line,_pos, _type, _text) {
	value = _value;
}

void TokenInteger::Print() {
	cout << line << '\t' << pos << '\t' << type << "\t\t" << text << "\t\t" << value << endl;
}

string TokenInteger::GetValue() {
	return value;
}

//---Real---
TokenReal::TokenReal(int _line, int _pos, string _type, string _text, string _value): Token(_line,_pos, _type, _text) {
	value = _value;
}

void TokenReal::Print() {
	cout << line << '\t' << pos << '\t' << type << "\t\t" << text << "\t\t" << value << endl;
}

string TokenReal::GetValue() {
	return value;
}

//---String---
TokenString::TokenString(int _line, int _pos, string _type, string _text, string _value): Token(_line,_pos, _type, _text) {
	value = _value;
}

void TokenString::Print() {
	cout << line << '\t' << pos << '\t' << type << "\t\t" << text << "\t\t" << value << endl;
}

string TokenString::GetValue() {
	return value;
}
