#include "parser.h"

Parser::Parser(Scanner *_scanner) {
	scanner = _scanner;
	Next();
	ParseExpression();
}

void Parser::Next() {
	scanner->Next();
	currentToken = scanner->GetToken();
}

//--- Parse Expression ----------
void Parser::ParseExpression() {
	Node *link = AssignmentExpr();
}

Node* Parser::AssignmentExpr() {
	Node *link = ConditionalExpr();
}

Node* Parser::ConditionalExpr() {
	Node *link = LogicalORExpr();
}
	
Node* Parser::LogicalORExpr() {
	Node *link = LogicalANDExpr();
}
	
Node* Parser::LogicalANDExpr() {
	Node *link = InclusiveORExpr();
}
	
Node* Parser::InclusiveORExpr() {
	Node *link = ExclusiveORExpr();
}

Node* Parser::ExclusiveORExpr() {
	Node *link = ANDExpr();
}
	
Node* Parser::ANDExpr() {
	Node *link = EqualityExpr();
}

Node* Parser::EqualityExpr() {
	Node *link = RelationalExpr();
}

Node* Parser::RelationalExpr() {
	Node *link = ShiftExpr();
}

Node* Parser::ShiftExpr() {
	Node *link = AdditiveExpr();
}

Node* Parser::AdditiveExpr() {
	Node *link = MultiplicativeExpr();
}

Node* Parser::MultiplicativeExpr() {
	Node *link = CastExpr();
}

Node* Parser::CastExpr() {
	Node *link = UnaryExpr();
}

Node* Parser::UnaryExpr() {
	Node *link = PostfixExpr();
}

Node* Parser::PostfixExpr() {
	Node *link = PrimaryExpr();
}

Node* Parser::PrimaryExpr() {
	//if(IsIdentifier(currentToken) || IsChar(currentToken) || IsString(currentToken)) 
		//return 
}

//--- Parse Statement ----------
void Parser::ParseStatement() {

}