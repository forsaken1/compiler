#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "node.h"

class Parser {
	Token *currentToken, *lastToken;
	Node* top;
	Scanner *scanner;
	
	void Next();

	Node* Expression():
	Node* AssignmentExpr();
	Node* ConditionalExpr();
	Node* BinaryOperationExpr(int);
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