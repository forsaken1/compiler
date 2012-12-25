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
public:
	ScannerException(string _msg): Exception(_msg) {}

	string GetMessage() {
		return "Error: " + msg;
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
		return "Syntax error: " + msg /* + " on line " + string(1, tk->GetLine() + '0') + " in position " + string(1, tk->GetPos() + '0')*/;
	}
};

class SemanticException: public ParserException {
public:
	SemanticException(Token *_tk, string _msg): ParserException(_tk, _msg) {}

	string GetMessage() {
		return "Semantic error: " + msg /* + " on line " + string(1, tk->GetLine() + '0') + " in position " + string(1, tk->GetPos() + '0')*/;
	}
};

#endif EXCEPTION_H