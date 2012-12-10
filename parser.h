#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "node.h"

class Parser {
	Token *currentToken;
	Scanner *scanner;
	
	void Next();

	Node* AssignmentExpr();
	Node* ConditionalExpr();
	Node* LogicalORExpr();
	Node* LogicalANDExpr();
	Node* InclusiveORExpr();
	Node* ExclusiveORExpr();
	Node* ANDExpr();
	Node* EqualityExpr();
	Node* RelationalExpr();
	Node* ShiftExpr();
	Node* AdditiveExpr();
	Node* MultiplicativeExpr();
	Node* CastExpr();
	Node* UnaryExpr();
	Node* PostfixExpr();
	Node* PrimaryExpr();

public:
	Parser(Scanner*);

	void ParseExpression();
	void ParseStatement();
};

#endif PARSER_H