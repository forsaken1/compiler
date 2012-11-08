#include "scanner.h"
#include <math.h>

Scanner::Scanner() {
	cout << "Scanner. Author: Krilov Alexey, C8303A" << endl << endl;
}

Scanner::Scanner(const char* fileName) {
	currentToken = NULL;
	currentString = "";
	currentPos = 0;
	currentLine = 1;
	currentChar = '\0';

	inputStream.open(fileName, ios::in);
	outputStream.open("output.txt", ios::out);
	getline(inputStream, currentString);

	InitKeyWordsTable();
	InitOperationsTable();
	InitSeparatorsTable();
	InitSpecialSymbolTable();
	
	Start();
}

Scanner::~Scanner() {
	inputStream.close();
	outputStream.close();
}

void Scanner::Start() {
	lastString = false;
	while(Next()) {}
}

bool Scanner::Next() {
	if(lastString) 
		return false;

	currentChar = GetChar();

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

	switch( GetCharType(currentChar) ) {
		case 1: currentToken = GetIdentificator(); break;
		case 2: currentToken = GetNumber(); break;
		case 3: currentToken = GetSymbol(); break;
		case 4: currentToken = GetString(); break;
		case 5: currentToken = GetSeparator(); break;
		case 6: currentToken = GetOperation(); break;
		case 7: return Next(); break;
		case 0: return Next(); break;
	}
	Print();
	delete currentToken; //for debug
	return true;
}

void Scanner::Print() {
	outputStream << currentToken->GetLine() << '\t'   << currentToken->GetPos() << '\t' 
				 << currentToken->GetType() << "\t\t" << currentToken->GetText() << endl;
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

int	Scanner::GetCharType(char ch) {
	if(IsLetter(ch))			return 1;
	if(IsNumber(ch))			return 2;
	if(IsCharSeparator(ch))		return 3;	
	if(IsStringSeparator(ch))	return 4;
	if(IsSeparator(ch))			return 5;
	if(IsSpecialSymbol(ch))		return 6;
	if(IsSpace(ch) || IsTabulationSymbol(ch) || IsEndOfLine(ch)) return 7;
	return 0;
}

Token* Scanner::GetToken()					{ return currentToken; }
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
	string s = "", type = "IDENTIF";
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
	if(IsKeyWord(s))
		type = "KEYWORD";

	return new Token(line, pos, type, s);
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
	string s = "", type = "INT";
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
		type = "REAL";
		return new TokenReal(line, pos, type, s, s);
	}
	return new TokenInteger(line, pos, type, s, s);
}

Token* Scanner::GetSymbol() {
	string s = "", type = "CHAR";
	int pos = currentPos, line = currentLine;

	while( (currentChar = GetChar()) != '\0' && !IsCharSeparator(currentChar)) {
		s += currentChar;
	}
	return new TokenString(line, pos, type, s, s);
}

Token* Scanner::GetString() {
	string s = "", type = "STRING";
	int pos = currentPos, line = currentLine;

	while((currentChar = GetChar()) != '\0' && !IsStringSeparator(currentChar)) {
		s += currentChar;
	}
	return new TokenString(line, pos, type, s, s);
}

Token* Scanner::GetSeparator() {
	string s = "", type = "SEPARAT";
	int pos = currentPos, line = currentLine;
	s += currentChar;

	return new Token(line, pos, type, s);
}

Token* Scanner::GetOperation() {
	string s = "", type = "OPERAT";
	int pos = currentPos, line = currentLine;
	s += currentChar;

	char lastChar = currentChar;

	if(IsOperation( string(1, lastChar) + string(1, GetChar()) )) {
		s += currentChar;
	}
	else
		BackToPreviousChar();

	return new Token(line, pos, type, s);
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
