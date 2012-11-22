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
	Print(p->left);
	cout << p->oper->GetText();
	Print(p->right);
}

void ProtoParser::Start() {
	Node *top = ParseExpression();
	Print(top);
	cout << endl;
}

bool IsNumber(Token *token) {
	return token->GetType() == "INT" || token->GetType() == "REAL";
}

bool IsAdditive(Token *token) {
	return token->GetText() == "+" || token->GetText() == "-";
}

bool IsMultiplicative(Token *token) {
	return token->GetText() == "*" || token->GetText() == "/";
}

bool ProtoParser::NextToken() {
	bool b = scanner->Next();
	token = scanner->GetToken();
	return b;
}

Node* ProtoParser::ParseExpression() {
	Node *left = NULL, *right = NULL;
	Token *tk = NULL;

	if(!NextToken()) return new Node(left, token, right);
	
	if(IsNumber(token))
		left = ParseTerm();

	if(!NextToken()) return left;

	if(IsAdditive(token)) {
		tk = token;
	}
	if(IsNumber(token)) {
		right = ParseTerm();
	}
	return new Node(left, token, right);
}

Node* ProtoParser::ParseTerm() {
	Node *left = NULL, *right = NULL;
	Token *tk = NULL;

	if(IsNumber(token))
		left = ParseFactor();
	
	if(!NextToken()) return left;
	if(IsAdditive(token)) return new Node(left, token, right);

	if(IsMultiplicative(token)) {
		NextToken();
		tk = token;
		right = ParseFactor();
	}
	return new Node(left, tk, right);
}

Node* ProtoParser::ParseFactor() {
	Node *left = NULL, *right = NULL;

	if(IsNumber(token))
		return new Node(left, token, right);
}