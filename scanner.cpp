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
	
	cout << "line\tpos\ttype\t\ttext\t\t\tvalue" << endl;
	Start();
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
	operation["::"] = true;
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
	operation["<<="] = true;
	operation[">>="] = true;
	operation["&="] = true;
	operation["|="] = true;
	operation["^="] = true;
	operation["->"] = true;
	operation["?"] = true;
	operation[":"] = true;
}

void Scanner::InitSeparatorsTable() {
	separator["("] = true;
	separator[")"] = true;
	separator["["] = true;
	separator["]"] = true;
	separator["{"] = true;
	separator["}"] = true;
	separator[";"] = true;
	separator["'"] = true;
	separator["\""] = true;
	separator[","] = true;
}

void Scanner::InitKeyWordsTable() {
	keyWord["and"] = true;
	keyWord["and_eq"] = true;
	keyWord["asm"] = true;
	keyWord["auto"] = true;
	keyWord["bitand"] = true;
	keyWord["bitor"] = true;
	keyWord["bool"] = true;
	keyWord["break"] = true;
	keyWord["case"] = true;
	keyWord["catch"] = true;
	keyWord["char"] = true;
	keyWord["class"] = true;
	keyWord["compl"] = true;
	keyWord["const"] = true;
	keyWord["const_char"] = true;
	keyWord["continue"] = true;
	keyWord["default"] = true;
	keyWord["delete"] = true;
	keyWord["do"] = true;
	keyWord["double"] = true;
	keyWord["dynamic_cast"] = true;
	keyWord["else"] = true;
	keyWord["enum"] = true;
	keyWord["explicit"] = true;
	keyWord["export"] = true;
	keyWord["extern"] = true;
	keyWord["false"] = true;
	keyWord["float"] = true;
	keyWord["for"] = true;
	keyWord["friend"] = true;
	keyWord["goto"] = true;
	keyWord["if"] = true;
	keyWord["inline"] = true;
	keyWord["int"] = true;
	keyWord["long"] = true;
	keyWord["mutable"] = true;
	keyWord["namespace"] = true;
	keyWord["new"] = true;
	keyWord["not"] = true;
	keyWord["not_eq"] = true;
	keyWord["operator"] = true;
	keyWord["or"] = true;
	keyWord["or_eq"] = true;
	keyWord["private"] = true;
	keyWord["protected"] = true;
	keyWord["public"] = true;
	keyWord["register"] = true;
	keyWord["reinterpret_cast"] = true;
	keyWord["return"] = true;
	keyWord["short"] = true;
	keyWord["signed"] = true;
	keyWord["sizeof"] = true;
	keyWord["static"] = true;
	keyWord["static_cast"] = true;
	keyWord["struct"] = true;
	keyWord["switch"] = true;
	keyWord["template"] = true;
	keyWord["this"] = true;
	keyWord["throw"] = true;
	keyWord["true"] = true;
	keyWord["try"] = true;
	keyWord["typedef"] = true;
	keyWord["typeid"] = true;
	keyWord["typename"] = true;
	keyWord["union"] = true;
	keyWord["unsigned"] = true;
	keyWord["using"] = true;
	keyWord["virtual"] = true;
	keyWord["void"] = true;
	keyWord["volatile"] = true;
	keyWord["wchar_t"] = true;
	keyWord["while"] = true;
	keyWord["xor"] = true;
	keyWord["xor_eq"] = true;
}

Scanner::~Scanner() {
	inputStream.close();
}

Token* Scanner::GetToken() {
	return currentToken;
}

void Scanner::Start() {
	while(Next()) {}
	cout << endl;
}

char Scanner::GetChar() {
	if(currentPos < currentString.length())
		return currentString[currentPos++];

	getline(inputStream, currentString);
	currentPos = 0;
	currentLine++;

	return '\0';
}

bool Scanner::IsKeyWord(string str) {
	if(keyWord[str])
		return true;
	return false;
}

bool Scanner::IsOperation(string str) {
	if(operation[str])
		return true;
	return false;
}

bool Scanner::IsSeparator(string str) {
	if(separator[str])
		return true;
	return false;
}

bool Scanner::IsLetter(char ch) {
	return 'a' <= ch && ch <= 'z' || ch == '_';
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
	return ('!' <= ch && ch <= '/') || (':' <= ch && ch <= '?') || ('[' <= ch && ch <= '^') || 
		   ('{' <= ch && ch <= '~') || ('\'' <= ch && ch <= '"') || 
		   ch == '<' || ch == '>' || ch == ',' || ch == '.';
}

bool Scanner::Next() {
	char currentChar = GetChar();

	if(currentChar == '\0') return false;
	if(IsSpace(currentChar) || currentChar == '\t') return true;

	string s = "", type;
	int pos = currentPos + 1, line = currentLine;
	s += currentChar;

	if(IsLetter(currentChar)) {	
		while((currentChar = GetChar()) != '\0' && !IsSpace(currentChar)) {
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

			while((currentChar = GetChar()) != '\0' && !IsSpace(currentChar)) {
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
				while((currentChar = GetChar()) != '\0' && !IsCharSeparator(currentChar)) {
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
				else {
					while((currentChar = GetChar()) != '\0' && !IsSpace(currentChar)) {
						if(!IsSeparator(currentChar + "")) {
							s += currentChar;
						}
						else {
							currentPos--;
							break;
						}
					}
					type = "separat";
				}

	currentToken = new Token(line, pos, type, s, NULL);
	currentToken->Print();
	
	return true;
}
