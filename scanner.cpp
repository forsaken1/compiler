#include "scanner.h"

Scanner::Scanner(const char* fileName) {
	currentToken = NULL;
	currentString = "";
	currentPos = 0;
	currentLine = 1;

	lastString = false;
	inputStream.open(fileName, ios::in);
	getline(inputStream, currentString);

	InitKeyWordsTable();
	InitOperationsTable();
	InitSeparatorsTable();
	InitSpecialSymbolTable();
	InitEscapeSequencesTable();
}

void Scanner::Start() {
	while(Next()) {
		currentToken->Print();
		delete currentToken; //for debug
	}
	inputStream.close();
}

bool Scanner::Next() {
	char currentChar;
	do {
		if(lastString) {
			currentToken = new Token(currentLine, currentPos, END_OF_FILE, "EOF", "");
			return false;
		}
		currentChar = GetChar();
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
	if(IsLetter(currentChar))				currentToken = GetIdentificator(currentChar); else 
	if(IsNumber(currentChar))				currentToken = GetNumber(currentChar); else 
	if(IsCharSeparator(currentChar))		currentToken = GetSymbol(currentChar); else 
	if(IsStringSeparator(currentChar))		currentToken = GetString(currentChar); else 
	if(IsSeparator(currentChar))			currentToken = GetSeparator(currentChar); else 
	if(IsSpecialSymbol(currentChar))		currentToken = GetOperation(currentChar); else 
	throw ScannerException("Indefinite character: \"" + string(1, currentChar) + "\"");

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

Token* Scanner::GetIdentificator(char currentChar) {
	string s = "";
	int pos = currentPos, line = currentLine;
	s += currentChar;

	while( !IsEndOfLine( currentChar = GetChar() ) && !IsSpace(currentChar) ) {
		if( IsLetter(currentChar) || IsNumber(currentChar) )
			s += currentChar;
		else {
			BackToPreviousChar();
			break;
		}
	}
	return new Token(line, pos, IsKeyWord(s) ? KEYWORD : IDENTIFIER, IsKeyWord(s) ? "KEYWORD" : "IDENTIF", s);
}

string Scanner::GetInvalidToken(int pos) {
	string str = "";
	for(int i = pos - 1; i < (int)currentString.length(); i++) {
		if(IsSpace(currentString[i]) || IsTabulationSymbol(currentString[i]))
			break;

		str += currentString[i];
	}
	return str;
}

Token* Scanner::GetNumber(char currentChar) {
	string s = "";
	int pos = currentPos, line = currentLine, dot = 0, E = 0;
	s += currentChar;

	while( (currentChar = GetChar()) != '\0' && !IsSpace(currentChar)) {
		if(dot && IsDot(currentChar)) 
			throw ScannerException("Too many dots in real number: \"" + GetInvalidToken(pos) + "\"");

		if(E) {
			if(IsE(currentChar)) 
				throw ScannerException("Too many symbol \"E\" in real number: \"" + GetInvalidToken(pos) + "\"");

			if(IsNumber(currentChar) || currentChar == '-' || currentChar == '+') {
				s += currentChar;
				continue;
			}
			else
				throw ScannerException("Invalid real number: \"" + GetInvalidToken(pos) + "\"");
		}

		if( IsNumber(currentChar) || IsE(currentChar) || IsDot(currentChar) ) {
			E += IsE(currentChar);
			dot += IsDot(currentChar);
			s += currentChar;
		}
		else {
			if(IsLetter(currentChar)) 
				throw ScannerException("Invalid identificator: \"" + GetInvalidToken(pos) + "\"");

			BackToPreviousChar();
			break;
		}
	}
	if(IsE(s[s.length() - 1]) || IsDot(s[s.length() - 1])) 
		throw ScannerException("Invalid real number: \"" + s + "\"");

	return new Token(line, pos, dot || E ? CONST_REAL : CONST_INTEGER, dot || E ? "REAL" : "INT", s);
}

Token* Scanner::GetSymbol(char currentChar) {
	string s = "";
	int pos = currentPos, line = currentLine;
	bool escapeDetector = false;
	currentChar = GetChar();
	s += currentChar;

	if(IsCharSeparator(currentChar))	throw ScannerException("Empty character constant");
	if(IsEndOfLine(currentChar))		throw ScannerException("Newline in character constant");
	if(IsTabulationSymbol(currentChar))	throw ScannerException("Tabulation symbol in character constant");

	if(currentChar == '\\') {
		escapeDetector = true;
		s += (currentChar = GetChar());
		if(!IsEscapeSequence(currentChar)) 
			throw ScannerException("Invalid ESCAPE-sequence: \"" + s + "\"");
	}

	if( IsCharSeparator(currentChar = GetChar()) )
		return new Token(line, pos, escapeDetector ? ESCAPE_SEQUENCE : CONST_CHAR, escapeDetector ? "ESCAPE" : "CHAR", s);
	else {
		s += currentChar;
		throw ScannerException("Too many long character constant: \"" + s + "\"");
	}
}

Token* Scanner::GetString(char currentChar) {
	string s = "";
	int pos = currentPos, line = currentLine;

	while((currentChar = GetChar()) != '\0' && !IsStringSeparator(currentChar)) {
		s += currentChar;
	}
	return new Token(line, pos, CONST_STRING, "STRING", s);
}

Token* Scanner::GetSeparator(char currentChar) {
	return new Token(currentLine, currentPos, SEPARATOR, "SEPARAT", string(1, currentChar));
}

Token* Scanner::GetOperation(char currentChar) {
	string s = "";
	char lastChar = currentChar;
	int pos = currentPos, line = currentLine;
	s += currentChar;
	currentChar = GetChar();

	if(!IsSpace(currentChar) && !IsTabulationSymbol(currentChar) && !IsEndOfLine(currentChar)) {
		if(IsOperation( string(1, lastChar) + string(1, currentChar) )) {
			s += currentChar;
		}
		else
			BackToPreviousChar();
	}
	return new Token(line, pos, OPERATOR, "OPERAT", s);
}

Token* Scanner::GetToken()					{ return currentToken; }
int  Scanner::IsDot(char ch)				{ return ch == '.' ? 1 : 0; }
int  Scanner::IsE(char ch)					{ return ch == 'E' || ch == 'e' ? 1 : 0; }
bool Scanner::IsKeyWord(string str)			{ return keyWord[str]; }
bool Scanner::IsOperation(string str)		{ return operation[str]; }
bool Scanner::IsEscapeSequence(char ch)		{ return escapeSequence[ch]; }
bool Scanner::IsSeparator(char ch)			{ return separator[ch]; }
bool Scanner::IsSpecialSymbol(char ch)		{ return specialSymbol[ch]; }
bool Scanner::IsLetter(char ch)				{ return ('A' <= ch && ch <= 'Z') || ('a' <= ch && ch <= 'z') || ch == '_'; }
bool Scanner::IsNumber(char ch)				{ return '0' <= ch && ch <= '9'; }
bool Scanner::IsSpace(char ch)				{ return ch == ' '; }
bool Scanner::IsCharSeparator(char ch)		{ return ch == '\''; }
bool Scanner::IsStringSeparator(char ch)	{ return ch == '"'; }
bool Scanner::IsTabulationSymbol(char ch)	{ return ch == '\t'; }
bool Scanner::IsEndOfLine(char ch)			{ return ch == '\0'; }
bool Scanner::IsCommentBegin(char ch)		{ return ch == '/'; }

void Scanner::BackToPreviousChar()			{ currentPos--; }
void Scanner::GoToNextChar()				{ currentPos++; }

void Scanner::InitEscapeSequencesTable() {
	escapeSequence['n'] = true;
	escapeSequence['t'] = true;
	escapeSequence['v'] = true;
	escapeSequence['b'] = true;
	escapeSequence['r'] = true;
	escapeSequence['f'] = true;
	escapeSequence['\''] = true;
	escapeSequence['"'] = true;
	escapeSequence['\\'] = true;
}

void Scanner::InitSeparatorsTable() {
	separator['('] = true;
	separator[')'] = true;
	separator['['] = true;
	separator[']'] = true;
	separator['{'] = true;
	separator['}'] = true;
	separator[';'] = true;
	separator[','] = true;
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
	specialSymbol['?'] = true;
	specialSymbol[':'] = true;
	specialSymbol['.'] = true;
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

void Scanner::InitKeyWordsTable() {
	keyWord["break"] = true;
	keyWord["char"] = true;
	keyWord["continue"] = true;
	keyWord["do"] = true;
	keyWord["else"] = true;
	keyWord["float"] = true;
	keyWord["for"] = true;
	keyWord["goto"] = true;
	keyWord["if"] = true;
	keyWord["int"] = true;
	keyWord["return"] = true;
	keyWord["sizeof"] = true;
	keyWord["struct"] = true;
	keyWord["void"] = true;
	keyWord["while"] = true;
	keyWord["print"] = true;
}
