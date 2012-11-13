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

	string GetInvalidToken(int);

public:
	Scanner();
	Scanner(const char*);

	Token* GetToken();
	void Start();
	bool Next();
};

#endif 
