#include "scanner.h"
#include <math.h>

Scanner::Scanner() {
	cout << "Scanner. Author: Krilov Alexey, C8303A" << endl << endl;
}

Scanner::Scanner(const char* _fileName) {
	currentToken = NULL;
	currentString = "";
	currentPos = 0;
	currentLine = 1;
	currentChar = '\0';
	fileName = _fileName;

	InitKeyWordsTable();
	InitOperationsTable();
	InitSeparatorsTable();
	InitSpecialSymbolTable();
	InitEscapeSequencesTable();
	
	Start();
}

void Scanner::Start() {
	lastString = false;
	inputStream.open(fileName, ios::in);
	outputStream.open("output.txt", ios::out);
	getline(inputStream, currentString);

	while(Next()) {
		currentToken->Print(outputStream);
		delete currentToken; //for debug
	}
	inputStream.close();
	outputStream.close();
}

bool Scanner::Next() {
	do {
		currentChar = GetChar();
		if(lastString) 
			return false;
	}
	while(IsSpace(currentChar) || IsTabulationSymbol(currentChar) || IsEndOfLine(currentChar));

	if(IsCommentBegin(currentChar)) {
		char secondChar;
		if( IsCommentBegin(secondChar = GetChar()) ) {
			NextLine();
			return Next();
		}
		if(secondChar == '*') {
			while( GetChar() != '*' || !IsCommentBegin(GetChar()) ) {} 
			return Next();
		}
	}
	if(IsLetter(currentChar))			currentToken = GetIdentificator();
	if(IsNumber(currentChar))			currentToken = GetNumber();
	if(IsCharSeparator(currentChar))	currentToken = GetSymbol();
	if(IsStringSeparator(currentChar))	currentToken = GetString();
	if(IsSeparator(currentChar))		currentToken = GetSeparator();
	if(IsSpecialSymbol(currentChar))	currentToken = GetOperation();
	return true;
}

char Scanner::GetChar() {
	if(currentPos < currentString.length())
		return currentString[currentPos++];

	NextLine();
	return '\0';
}

void Scanner::NextLine() {
	if(inputStream.eof()) {
		lastString = true;
		return;
	}
	getline(inputStream, currentString);
	currentPos = 0;
	currentLine++;
}

Token* Scanner::GetToken()					{ return currentToken; }
bool Scanner::IsEscapeSequence(string str)  { return escapeSequence[str]; }
bool Scanner::IsKeyWord(string str)			{ return keyWord[str]; }
bool Scanner::IsOperation(string str)		{ return operation[str]; }
bool Scanner::IsSeparator(char ch)			{ return separator[ch]; }
bool Scanner::IsSpecialSymbol(char ch)		{ return specialSymbol[ch]; }
bool Scanner::IsLetter(char ch)				{ return ('a' <= ch && ch <= 'z') || ch == '_'; }
bool Scanner::IsNumber(char ch)				{ return '0' <= ch && ch <= '9'; }
bool Scanner::IsDot(char ch)				{ return ch == '.'; }
bool Scanner::IsSpace(char ch)				{ return ch == ' '; }
bool Scanner::IsCharSeparator(char ch)		{ return ch == '\''; }
bool Scanner::IsStringSeparator(char ch)	{ return ch == '"'; }
bool Scanner::IsTabulationSymbol(char ch)	{ return ch == '\t'; }
bool Scanner::IsEndOfLine(char ch)			{ return ch == '\0'; }
bool Scanner::IsCommentBegin(char ch)		{ return ch == '/'; }

void Scanner::BackToPreviousChar() {
	currentPos--;
}

Token* Scanner::GetIdentificator() {
	string s = "";
	int pos = currentPos, line = currentLine;
	s += currentChar;

	while( !IsEndOfLine(currentChar = GetChar()) && !IsSpace(currentChar) ) {
		if( IsLetter(currentChar) || IsNumber(currentChar) )
			s += currentChar;
		else {
			BackToPreviousChar();
			break;
		}
	}
	return new Token(line, pos, IsKeyWord(s) ? "KEYWORD" : "IDENTIF", s);
}
/* //making...
int CharToInt(char ch) {
	return ch - 48;
}

int StrToInt(string s) {
	int result = 0;
	for(int i = 0; i < s.length(); i++) {
		result += CharToInt(s[i]) * (int)pow((float)10, i);
	}
	return result;
}
*/
Token* Scanner::GetNumber() {
	string s = "";
	int pos = currentPos, line = currentLine, dotCount = 0;
	bool dot = false;
	s += currentChar;

	while( (currentChar = GetChar()) != '\0' && !IsSpace(currentChar)) {
		if(IsNumber(currentChar) || ( dot = IsDot(currentChar) ))
			s += currentChar;
		else {
			BackToPreviousChar();
			break;
		}
	}
	if(dot) {
		return new TokenReal(line, pos, "REAL", s, s);
	}
	return new TokenInteger(line, pos, "INT", s, s);
}

Token* Scanner::GetSymbol() {
	string s = "";
	int pos = currentPos, line = currentLine;

	while( (currentChar = GetChar()) != '\0' && !IsCharSeparator(currentChar)) {
		s += currentChar;
	}
	if(IsEscapeSequence(s)) {
		return new TokenString(line, pos, "ESCAPE", s, s);
	}
	return new TokenString(line, pos, "CHAR", s, s);
}

Token* Scanner::GetString() {
	string s = "";
	int pos = currentPos, line = currentLine;

	while((currentChar = GetChar()) != '\0' && !IsStringSeparator(currentChar)) {
		s += currentChar;
	}
	return new TokenString(line, pos, "STRING", s, s);
}

Token* Scanner::GetSeparator() {
	return new Token(currentLine, currentPos, "SEPARAT", string(1, currentChar));
}

Token* Scanner::GetOperation() {
	string s = "";
	char lastChar = currentChar;
	int pos = currentPos, line = currentLine;
	s += currentChar;

	if(IsOperation( string(1, lastChar) + string(1, GetChar()) )) {
		s += currentChar;
	}
	else
		BackToPreviousChar();

	return new Token(line, pos, "OPERAT", s);
}

void Scanner::InitEscapeSequencesTable() {
	escapeSequence["\n"] = true;
	escapeSequence["\t"] = true;
	escapeSequence["\v"] = true;
	escapeSequence["\b"] = true;
	escapeSequence["\r"] = true;
	escapeSequence["\f"] = true;
	escapeSequence["\'"] = true;
	escapeSequence["\\"] = true;
}

void Scanner::InitSpecialSymbolTable() {
	specialSymbol['='] = true;
	specialSymbol['+'] = true;
	specialSymbol['-'] = true;
	specialSymbol['>'] = true;
	specialSymbol['<'] = true;
	specialSymbol['&'] = true;
	specialSymbol['|'] = true;
	specialSymbol['!'] = true;
	specialSymbol['%'] = true;
	specialSymbol['^'] = true;
	specialSymbol['*'] = true;
	specialSymbol['/'] = true;
}

void Scanner::InitOperationsTable() {
	operation["+"] = true;
	operation["-"] = true;
	operation["*"] = true;
	operation["/"] = true;
	operation["=="] = true;
	operation[">="] = true;
	operation["<="] = true;
	operation["!="] = true;
	operation["="] = true;
	operation["%"] = true;
	operation["<"] = true;
	operation[">"] = true;
	operation["!"] = true;
	operation["."] = true;
	operation["++"] = true;
	operation["--"] = true;
	operation[">>"] = true;
	operation["<<"] = true;
	operation["&"] = true;
	operation["~"] = true;
	operation["|"] = true;
	operation["||"] = true;
	operation["&&"] = true;
	operation["^"] = true;
	operation["+="] = true;
	operation["-="] = true;
	operation["*="] = true;
	operation["/="] = true;
	operation["%="] = true;
	operation["&="] = true;
	operation["|="] = true;
	operation["^="] = true;
	operation["->"] = true;
	operation["?"] = true;
	operation[":"] = true;
}

void Scanner::InitSeparatorsTable() {
	separator['('] = true;
	separator[')'] = true;
	separator['['] = true;
	separator[']'] = true;
	separator['{'] = true;
	separator['}'] = true;
	separator[';'] = true;
	separator['\''] = true;
	separator['"'] = true;
	separator[','] = true;
}

void Scanner::InitKeyWordsTable() {
	keyWord["asm"] = true;
	keyWord["auto"] = true;
	keyWord["break"] = true;
	keyWord["case"] = true;
	keyWord["char"] = true;
	keyWord["const"] = true;
	keyWord["continue"] = true;
	keyWord["default"] = true;
	keyWord["do"] = true;
	keyWord["double"] = true;
	keyWord["else"] = true;
	keyWord["enum"] = true;
	keyWord["extern"] = true;
	keyWord["false"] = true;
	keyWord["float"] = true;
	keyWord["for"] = true;
	keyWord["goto"] = true;
	keyWord["if"] = true;
	keyWord["inline"] = true;
	keyWord["int"] = true;
	keyWord["long"] = true;
	keyWord["register"] = true;
	keyWord["return"] = true;
	keyWord["short"] = true;
	keyWord["signed"] = true;
	keyWord["sizeof"] = true;
	keyWord["static"] = true;
	keyWord["struct"] = true;
	keyWord["switch"] = true;
	keyWord["true"] = true;
	keyWord["typedef"] = true;
	keyWord["union"] = true;
	keyWord["unsigned"] = true;
	keyWord["void"] = true;
	keyWord["volatile"] = true;
	keyWord["while"] = true;
}
