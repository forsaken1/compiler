#include "parser.h"

Parser::Parser(Scanner *_scanner) {
	scanner = _scanner;
	currentToken = NULL;
	lastToken = NULL;

	Parse();
}

void Parser::Next() {
	if(scanner->EoF())
		throw ParserException("Syntax analisys success");

	scanner->Next();
	lastToken = currentToken;
	currentToken = scanner->GetToken();
	oper = currentToken->GetText();
}

//--- Parse Expression ----------

void Parser::Parse() {
	try {
		top = Expression();
	}
	catch(ParserException &e) {
		cout << e.GetMessage() << endl;
	}
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

	string oper = currentToken->GetText();

	if(oper == "++" || oper == "--" || oper == "sizeof") { //unary-expr
		Next();
		oper = currentToken->GetText();
		if(assignmentOperator[oper]) {
			Next();
			Node *right = AssignmentExpr();
			return ;
		}
	}
	if(unaryOperator[oper]) { //cast-expr

	}
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
		case 6: condition = oper == "<"  || oper == ">" || oper == "<=" || oper == ">="; break;
		case 7: condition = oper == ">>" || oper == "<<"; break;
		case 8: condition = oper == "+"  || oper == "-"; break;
		case 9: condition = oper == "*"  || oper == "/" || oper == "%"; break;
	}
	if(condition) {
		Next();
		Node *right = BinaryOperationExpr(priority);
		return new NodeBinary(left, oper, right);
	}
	return left;
}

Node* Parser::CastExpr() {
	Node *left = NULL;

	if(oper == "(") {
		Next();
		
	}
	left = UnaryExpr();

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
	return left;
}

Node* Parser::PrimaryExpr() {
	if(currentToken->GetType() == IDENTIFIER) {
		Next();
		return new NodeVar(lastToken->GetText());
	}

	if(currentToken->GetType() == CONST_INTEGER ||
	   currentToken->GetType() == CONST_REAL ||
	   currentToken->GetType() == CONST_CHAR ||
	   currentToken->GetType() == CONST_STRING) {
		Next();
		return new NodeConst(lastToken->GetText());
	}

	Next();
	Node *expr = NULL;

	if(currentToken->GetText() == "(") {
		expr = Expression();
		Next();
		if(currentToken->GetText() == ")") {
			Next();
			return expr;
		}
	}
}

//--- Parse Statement ----------

Node* Parser::Statement() {

	return NULL;
}

Node* Parser::CompoundStmt() {

	return NULL;
}

Node* Parser::ExpressionStmt() {

	return NULL;
}

Node* Parser::DeclarationStmt() {

	return NULL;
}

Node* Parser::SelectionStmt() {

	return NULL;
}

Node* Parser::IterationStmt() {

	return NULL;
}

Node* Parser::FunctionDefinitionStmt() {

	return NULL;
}

Node* Parser::JumpStmt() {

	return NULL;
}

//--- Parse Definition ---

void Parser::InitHashes() {
	assignmentOperator["="] = true;
	assignmentOperator["*="] = true;
	assignmentOperator["/="] = true;
	assignmentOperator["%="] = true;
	assignmentOperator["+="] = true;
	assignmentOperator["-="] = true;
	assignmentOperator["&="] = true;
	assignmentOperator["^="] = true;
	assignmentOperator["|="] = true;

	unaryOperator["&"] = true;
	unaryOperator["*"] = true;
	unaryOperator["+"] = true;
	unaryOperator["-"] = true;
	unaryOperator["~"] = true;
	unaryOperator["!"] = true;
}