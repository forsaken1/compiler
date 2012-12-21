#ifndef SCANNER_H
#define SCANNER_H

#include <map>
#include <fstream>
#include "token.h"

class Scanner {
	map<string, bool> keyWord, operation;
	map<char, bool> separator, specialSymbol, escapeSequence;
	Token* currentToken;
	string currentString;
	ifstream inputStream;
	unsigned int currentPos, currentLine;
	const char* fileName;
	bool lastString;

	void NextLine();
	void InitKeyWordsTable();
	void InitOperationsTable();
	void InitSeparatorsTable();
	void InitSpecialSymbolTable();
	void InitEscapeSequencesTable();
	void BackToPreviousChar();
	void GoToNextChar();

	Token* GetIdentificator(char);
	Token* GetNumber(char); 
	Token* GetSymbol(char);
	Token* GetString(char); 
	Token* GetSeparator(char);
	Token* GetOperation(char); 
	
	int  IsDot(char);	
	int  IsE(char);
	bool IsLetter(char);
	bool IsNumber(char);
	bool IsCharSeparator(char);
	bool IsStringSeparator(char);
	bool IsSpace(char);
	bool IsTabulationSymbol(char);
	bool IsEndOfLine(char);
	bool IsCommentBegin(char);
	bool IsSeparator(char);
	bool IsSpecialSymbol(char);
	bool IsEscapeSequence(char);
	bool IsKeyWord(string);
	bool IsOperation(string);
	
	char GetChar();

	string GetInvalidToken(int);

public:
	Scanner(const char*);

	Token* GetToken();
	void Start();
	bool Next();
	bool EoF();
};

class ScannerException {
	string msg;

public:
	ScannerException(string _msg) {
		msg = _msg;
	}

	string GetMessage() {
		return "Error: " + msg;
	}
};

#endif SCANNER_H
