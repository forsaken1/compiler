#include "token.h"

Token::Token(int _line, int _pos, string _type, string _text, string* _value) {
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

int Token::GetValueInt() {
	return valueInt;
}

float Token::GetValueFloat() {
	return valueFloat;
}
