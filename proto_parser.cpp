#include "proto_parser.h"

ProtoParser::ProtoParser(Scanner* _scanner) {
	scanner = _scanner;
	token = new Token(0, 0, "", "");
	Start();
}

void Print(Node *p) {
	if(p == NULL) {
		return;
	}
	cout << "(";
	Print(p->left);
	cout << " " << p->oper->GetText() << " ";
	Print(p->right);
	cout << ")";
}

void ProtoParser::Start() {
	NextToken();
	Node *top = ParseExpression();
	Print(top);
}

bool IsNumber(Token *token) {
	if(token == NULL) return false;
	return token->GetType() == "INT" || token->GetType() == "REAL";
}

bool IsAdditive(Token *token) {
	if(token == NULL) return false;
	return token->GetText() == "+" || token->GetText() == "-";
}

bool IsMultiplicative(Token *token) {
	if(token == NULL) return false;
	return token->GetText() == "*" || token->GetText() == "/";
}

bool IsLeftBracket(Token *token) {
	if(token == NULL) return false;
	return token->GetText() == "(";
}

bool IsRightBracket(Token *token) {
	if(token == NULL) return false;
	return token->GetText() == ")";
}

bool ProtoParser::NextToken() {
	bool b = scanner->Next();
	token = scanner->GetToken();
	return b;
}

Node* ProtoParser::ParseExpression() {
	Node *left = NULL, *right = NULL;
	Token *tk = NULL;
	
	if(IsNumber(token) || IsLeftBracket(token))
		left = ParseTerm();
	
	if(scanner->EoF())
		return left;

	if(IsAdditive(token)) {
		tk = token;
		NextToken();
		right = ParseTerm();
	}
	return new Node(left, tk, right);
}

Node* ProtoParser::ParseTerm() {
	Node *left = NULL, *right = NULL;
	Token *tk = NULL;

	if(IsNumber(token) || IsLeftBracket(token))
		left = ParseFactor();

	if(IsAdditive(token) || IsRightBracket(token) || scanner->EoF())
		return left;

	if(IsMultiplicative(token)) {
		tk = token;
		NextToken();
		right = ParseFactor();
	}
	return new Node(left, tk, right);
}

Node* ProtoParser::ParseFactor() {
	Node *left = NULL, *right = NULL, *t;
	Token *tk = NULL;

	if(IsNumber(token)) {
		tk = token;
		NextToken();
		return new Node(left, tk, right);
	}
	if(IsLeftBracket(token)) {
		NextToken();
		t = ParseExpression();
	}
	if(IsRightBracket(token)) {
		NextToken();
		return t;
	}
}