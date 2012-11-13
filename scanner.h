#ifndef SCANNER_H
#define SCANNER_H

#include <map>
#include "token.h"

class Scanner {
	map<string, bool> keyWord, operation, escapeSequence;
	map<char, bool> separator, specialSymbol;
	Token* currentToken;
	string currentString;
	ifstream inputStream;
	ofstream outputStream;
	unsigned int currentPos, currentLine;
	char currentChar;
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

	Token* GetIdentificator();
	Token* GetNumber(); 
	Token* GetSymbol();
	Token* GetString(); 
	Token* GetSeparator();
	Token* GetOperation(); 
	
	bool IsLetter(char);
	bool IsNumber(char);
	bool IsDot(char);	
	bool IsE(char);
	bool IsCharSeparator(char);
	bool IsStringSeparator(char);
	bool IsSpace(char);
	bool IsTabulationSymbol(char);
	bool IsEndOfLine(char);
	bool IsCommentBegin(char);
	bool IsSeparator(char);
	bool IsSpecialSymbol(char);
	bool IsKeyWord(string);
	bool IsOperation(string);
	bool IsEscapeSequence(string);
	
	char GetChar();

public:
	Scanner();
	Scanner(const char*);

	Token* GetToken();
	void Start();
	bool Next();
};

#endif 
