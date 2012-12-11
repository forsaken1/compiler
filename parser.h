#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "node.h"

class Parser {
	Token *currentToken, *lastToken;
	Node *top;
	Scanner *scanner;
	
	void Next();

	Node* Expression();
	Node* AssignmentExpr();
	Node* ConditionalExpr();
	Node* BinaryOperationExpr(int);
	Node* CastExpr();
	Node* UnaryExpr();
	Node* PostfixExpr();
	Node* PrimaryExpr();

	Node* Statement();
	Node* CompoundStmt();
	Node* ExpressionStmt();
	Node* DeclarationStmt();
	Node* SelectionStmt();
	Node* IterationStmt();
	Node* FunctionDefinitionStmt();
	Node* JumpStmt();

public:
	Parser(Scanner*);

	void ParseExpression();
	void ParseStatement();
};

#endif PARSER_H