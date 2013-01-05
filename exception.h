#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception {
protected:
	string msg;

public:
	Exception(string _msg) {
		msg = _msg;
	}
	virtual string GetMessage() { return ""; }
};

class ScannerException: public Exception {
	int line, pos;

public:
	ScannerException(int _line, int _pos, string _msg): Exception(_msg) {
		line = _line;
		pos = _pos;
	}

	string GetMessage() {
		return "Lexical error: " + msg + " on line " + to_string((_Longlong)line) + " in position " + to_string((_Longlong)pos);
	}
};

class ParserException: public Exception {
protected:
	Token *tk;

public:
	ParserException(Token *_tk, string _msg): Exception(_msg) {
		tk = _tk;
	}

	string GetMessage() {
		return "Syntax error: " + msg + " on line " + to_string((_Longlong)tk->GetLine()) + " in position " + to_string((_Longlong)tk->GetPos());
	}
};

class SemanticException: public ParserException {
public:
	SemanticException(Token *_tk, string _msg): ParserException(_tk, _msg) {}

	string GetMessage() {
		return "Semantic error: " + msg + " on line " + to_string((_Longlong)tk->GetLine()) + " in position " + to_string((_Longlong)tk->GetPos());
	}
};

#endif EXCEPTION_H