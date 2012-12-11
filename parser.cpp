#include "parser.h"

Parser::Parser(Scanner *_scanner) {
	scanner = _scanner;
	currentToken = NULL;
	lastToken = NULL;
	top = NULL;

	ParseExpression();
}

void Parser::Next() {
	scanner->Next();
	lastToken = currentToken;
	currentToken = scanner->GetToken();
}

//--- Parse Expression ----------
void Parser::ParseExpression() {
	Next();
	top = Expression();
}

Node* Parser::Expression() {
	Node *left = AssignmentExpr();

	if(currentToken->GetText() == ",") {
		Next();
		Node *right = Expression();
	}
	return left;
}

Node* Parser::AssignmentExpr() {
	Node *left = ConditionalExpr();

	return left;
}

Node* Parser::ConditionalExpr() { 
	Node *left = BinaryOperationExpr(0);

	return left;
}

Node* Parser::BinaryOperationExpr(int priority) {
	Node *left = NULL;

	if(priority == 9)
		left = CastExpr();
	else
		left = BinaryOperationExpr(priority + 1);

	string oper = currentToken->GetText();
	bool condition = false;

	switch(priority) {
		case 0: condition = oper == "||"; break;
		case 1: condition = oper == "&&"; break;
		case 2: condition = oper == "|"; break;
		case 3: condition = oper == "^"; break;
		case 4: condition = oper == "&"; break;
		case 5: condition = oper == "==" || oper == "!="; break;
		case 6: condition = oper == "<" || oper == ">" || oper == "<=" || oper == ">="; break;
		case 7: condition = oper == ">>" || oper == "<<"; break;
		case 8: condition = oper == "+" || oper == "-"; break;
		case 9: condition = oper == "*" || oper == "/" || oper == "%"; break;
	}
	if(condition) {
		Next();
		Node *right = BinaryOperationExpr(priority);
		return new NodeBinary(left, oper, right);
	}
	return left;
}

Node* Parser::CastExpr() {
	Node *left = UnaryExpr();

	return left;
}

Node* Parser::UnaryExpr() {
	Node *left = PostfixExpr();

	return left;
}

Node* Parser::PostfixExpr() {
	Node *left = PrimaryExpr();

	if(currentToken->GetText() == "[") {

	}
	if(currentToken->GetText() == "(") {

	}
	if(currentToken->GetText() == ".") {

	}
	if(currentToken->GetText() == "->") {

	}
	if(currentToken->GetText() == "++" || currentToken->GetText() == "--") {

	}
}

Node* Parser::PrimaryExpr() { //класс для констант
	if(currentToken->GetType() == IDENTIFIER)
		return new NodeVar(currentToken->GetText());

	//if(currentToken->GetType() == CONST_CHAR ||
	  // currentToken->GetType() == CONST_STRING)

}

//--- Parse Statement ----------
void Parser::ParseStatement() {

}