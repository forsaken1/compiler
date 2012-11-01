#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <map>
#include "token.h"

class Scanner {
	map<string, bool> keyWord, operation, separator;
	Token* currentToken;
	string fileName, currentString;
	ifstream inputStream;
	unsigned int currentPos, currentLine;

public:
	Scanner();
	Scanner(string);
	~Scanner();

	Token* GetToken();
	void Start();
	void InitKeyWordsTable();
	void InitOperationsTable();
	void InitSeparatorsTable();

	char GetChar();
	bool Next();

	bool IsLetter(char);
	bool IsNumber(char);
	bool IsDot(char);	
	bool IsCharSeparator(char);
	bool IsStringSeparator(char);
	bool IsSpace(char);
	bool IsSpecialSymbol(char);

	bool IsKeyWord(string);
	bool IsOperation(string);
	bool IsSeparator(string);
};

#endif 