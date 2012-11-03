#include "scanner.h"

Scanner::Scanner() {
	cout << "Scanner. Author: Krilov Alexey, C8303A" << endl << endl;
}

Scanner::Scanner(string _fileName) {
	currentToken = NULL;
	currentString = "";
	currentPos = 0;
	currentLine = 1;

	fileName = _fileName;
	inputStream.open("input.txt", ios::in);
	getline(inputStream, currentString);

	InitKeyWordsTable();
	InitOperationsTable();
	InitSeparatorsTable();
	
	Start();
}

Scanner::~Scanner() {
	inputStream.close();
}

Token* Scanner::GetToken() {
	return currentToken;
}

void Scanner::Start() {
	cout << "line\tpos\ttype\t\ttext\t\t\tvalue" << endl << endl;
	while(Next()) {}
	cout << endl;
}

char Scanner::GetChar() {
	if(currentPos < currentString.length())
		return currentString[currentPos++];

	NextLine();
	return '\0';
}

void Scanner::NextLine() {
	if(inputStream.eof()) return;
	getline(inputStream, currentString);
	currentPos = 0;
	currentLine++;
}

bool Scanner::IsKeyWord(string str) {
	return keyWord[str];
}

bool Scanner::IsOperation(string str) {
	return operation[str];
}

bool Scanner::IsSeparator(char ch) {
	return separator[ch];
}

bool Scanner::IsLetter(char ch) {
	return ('a' <= ch && ch <= 'z') || ch == '_';
}

bool Scanner::IsNumber(char ch) {
	return '0' <= ch && ch <= '9';
}

bool Scanner::IsDot(char ch) {
	return ch == '.';
}

bool Scanner::IsSpace(char ch) {
	return ch == ' ';
}

bool Scanner::IsCharSeparator(char ch) {
	return ch == '\'';
}

bool Scanner::IsStringSeparator(char ch) {
	return ch == '"';
}

bool Scanner::IsSpecialSymbol(char ch) {
	/*return ('!' <= ch && ch <= '/') || (':' <= ch && ch <= '?') || ('[' <= ch && ch <= '^') || 
		   ('{' <= ch && ch <= '~') || ('\'' <= ch && ch <= '"') || 
		   ch == '<' || ch == '>' || ch == ',' || ch == '.';*/

	return ch == '=' || ch == '+' || ch == '-' || ch == '>' || ch == '<' || ch == '&' || ch == '|';
}

bool Scanner::Next() {
	char currentChar = GetChar();

	if(inputStream.eof()) return false;

	if(IsSpace(currentChar) || currentChar == '\t' || currentChar == '\0') return true;

	if(currentChar == '/') {
		if( (currentChar = GetChar()) == '/') {
			NextLine();
			return true;
		}
		if(currentChar == '*') {
			while( GetChar() != '*' || GetChar() != '/') {} 
			return true;
		}
	}

	string s = "", type;
	int pos = currentPos, line = currentLine;
	s += currentChar;

	if(IsLetter(currentChar)) {	
		while( (currentChar = GetChar()) != '\0' && !IsSpace(currentChar)) {
			if(IsLetter(currentChar) || IsNumber(currentChar))
				s += currentChar;
			else {
				currentPos--;
				break;
			}
		}
		if(IsKeyWord(s))
			type = "keyword";
		else
			type = "identif";
	}
	else
		if(IsNumber(currentChar)) {
			bool dot = false;
			int dotCount = 0;

			while( (currentChar = GetChar()) != '\0' && !IsSpace(currentChar)) {
				if(IsNumber(currentChar) || ( dot = IsDot(currentChar) ))
					s += currentChar;
				else {
					currentPos--;
					break;
				}
			}
			if(dot)
				type = "real";
			else
				type = "int";
		}
		else 
			if(IsCharSeparator(currentChar)) {
				s = "";
				while( (currentChar = GetChar()) != '\0' && !IsCharSeparator(currentChar)) {
					s += currentChar;
				}
				type = "char";
			}
			else 
				if(IsStringSeparator(currentChar)) {
					s = "";
					while((currentChar = GetChar()) != '\0' && !IsStringSeparator(currentChar)) {
						s += currentChar;
					}
					type = "string";
				}
				else 
					if(IsSeparator(currentChar)) {
						type = "separat";
					}
					else {
						char lastChar = currentChar;
						type = "operat";
						if( !IsSpecialSymbol(currentChar = GetChar()) )
							if(IsOperation( string(1, lastChar) + string(1, currentChar) )) {
								s += currentChar;
							}
							else
								if( IsOperation(string(1, currentChar)) ) {
									type = "operat";
								}
					}

	currentToken = new Token(line, pos, type, s, NULL);
	currentToken->Print();
	
	return true;
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
