#include "parser.h"

Parser::Parser(Scanner *_scanner) {
	scanner = _scanner;
	currentToken = NULL;
	lastToken = NULL;

	Next();
	ParseExpression();
}

void Parser::Next() {
	scanner->Next();
	lastToken = currentToken;
	currentToken = scanner->GetToken();
}

//--- Parse Expression ----------
void Parser::ParseExpression() {
	Node *left = AssignmentExpr();
	Next();
	if(currentToken->GetText() == ",")
		Node *right = AssignmentExpr();
}

Node* Parser::AssignmentExpr() {
	Node *left = ConditionalExpr();

}


Node* Parser::ConditionalExpr() { //нужен класс для тернарной операции
	Node *left = BinaryOperationExpr(0);
	/*Next();
	string oper = currentToken->GetText();
	if(oper == "?") {
		Next();
		Node *node 
	}*/
}

Node* Parser::BinaryOperationExpr(int priority) {
	if(priority > 9)
		Node* left = CastExpr();
	else
		Node* left = BinaryOperationExpr(priority + 1);

	Next();
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
}

	/*
Node* Parser::LogicalORExpr() {
	Node *left = LogicalANDExpr();
	Next();
	string oper = currentToken->GetText();
	if(oper == "||") {
		Next();
		Node *right = LogicalORExpr();
		return new NodeBinary(left, oper, right);
	}
	else
		return left;
}
	
Node* Parser::LogicalANDExpr() {
	Node *left = InclusiveORExpr();
	Next();
	string oper = currentToken->GetText();
	if(oper == "&&") {
		Next();
		Node *right = LogicalANDExpr();
		return new NodeBinary(left, oper, right);
	}
	else
		return left;
}
	
Node* Parser::InclusiveORExpr() {
	Node *left = ExclusiveORExpr();
	Next();
	string oper = currentToken->GetText();
	if(oper == "|") {
		Next();
		Node *right = InclusiveORExpr();
		return new NodeBinary(left, oper, right);
	}
	else
		return left;
}

Node* Parser::ExclusiveORExpr() {
	Node *left = ANDExpr();
	Next();
	string oper = currentToken->GetText();
	if(oper == "^") {
		Next();
		Node *right = ExclusiveORExpr();
		return new NodeBinary(left, oper, right);
	}
	else
		return left;
}
	
Node* Parser::ANDExpr() {
	Node *left = EqualityExpr();
	Next();
	string oper = currentToken->GetText();
	if(oper == "&") {
		Next();
		Node *right = ANDExpr();
		return new NodeBinary(left, oper, right);
	}
	else
		return left;
}

Node* Parser::EqualityExpr() {
	Node *left = RelationalExpr();
	Next();
	string oper = currentToken->GetText();
	if(oper == "==" || oper == "!=") {
		Next();
		Node *right = EqualityExpr();
		return new NodeBinary(left, oper, right);
	}
	else
		return left;
}

Node* Parser::RelationalExpr() {
	Node *left = ShiftExpr();
	Next();
	string oper = currentToken->GetText();
	if(oper == ">" || oper == "<" || oper == ">=" || oper == "<=") {
		Next();
		Node *right = RelationalExpr();
		return new NodeBinary(left, oper, right);
	}
	else
		return left;
}

Node* Parser::ShiftExpr() {
	Node *left = AdditiveExpr();
	Next();
	string oper = currentToken->GetText();
	if(oper == ">>" || oper == "<<") {
		Next();
		Node *right = ShiftExpr();
		return new NodeBinary(left, oper, right);
	}
	else
		return left;
}

Node* Parser::AdditiveExpr() {
	Node *left = MultiplicativeExpr();
	Next();
	string oper = currentToken->GetText();
	if(oper == "+" || oper == "-") {
		Next();
		Node *right = AdditiveExpr();
		return new NodeBinary(left, oper, right);
	}
	else
		return left;
}

Node* Parser::MultiplicativeExpr() {
	Node *left = CastExpr();
	Next();
	string oper = currentToken->GetText();
	if(oper == "*" || oper == "/" || oper == "%") {
		Next();
		Node *right = MultiplicativeExpr();
		return new NodeBinary(left, oper, right);
	}
	else
		return left;
}*/

Node* Parser::CastExpr() {
	Node *left = UnaryExpr();
}

Node* Parser::UnaryExpr() {
	Node *left = PostfixExpr();
}

Node* Parser::PostfixExpr() {
	Node *left = PrimaryExpr();
	Next();
	if(currentToken->GetText() == "[") {
		Next();
	}
	if(currentToken->GetText() == "(") {

	}
	if(currentToken->GetText() == ".") {

	}
	if(currentToken->GetText() == "->") {

	}
	if(currentToken->GetText() == "++" || currentToken->GetText() == "--") {
		return ;
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