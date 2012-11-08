#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <map>
#include "token.h"

class Scanner {
	map<string, bool> keyWord, operation;
	map<char, bool> separator, specialSymbol;
	Token* currentToken;
	string fileName, currentString;
	ifstream inputStream;
	ofstream outputStream;
	unsigned int currentPos, currentLine;
	char currentChar;
	bool lastString;

public:
	Scanner();
	Scanner(const char*);
	~Scanner();

	Token* GetToken();
	void Start();
	void NextLine();
	void Print();
	void InitKeyWordsTable();
	void InitOperationsTable();
	void InitSeparatorsTable();
	void InitSpecialSymbolTable();
	void BackToPreviousChar();
	
	Token* GetIdentificator();
	Token* GetNumber(); 
	Token* GetSymbol();
	Token* GetString(); 
	Token* GetSeparator();
	Token* GetOperation(); 

	int  GetCharType(char);
	char GetChar();
	bool Next();

	bool IsLetter(char);
	bool IsNumber(char);
	bool IsDot(char);	
	bool IsCharSeparator(char);
	bool IsStringSeparator(char);
	bool IsSpace(char);
	bool IsSpecialSymbol(char);
	bool IsTabulationSymbol(char);
	bool IsEndOfLine(char);
	bool IsCommentBegin(char);

	bool IsKeyWord(string);
	bool IsOperation(string);
	bool IsSeparator(char);
};

#endif 