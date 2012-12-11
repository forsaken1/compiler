#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "node.h"

class Parser {
	Token *currentToken, *lastToken;
	Node *top;
	Scanner *scanner;
	
	Node* Parse();

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
	
	void Next();

public:
	Parser(Scanner*);
};

class ParserException {
	string msg;

public:
	ParserException(string _msg) {
		msg = _msg;
	}

	string GetMessage() {
		return "Syntax error: " + msg;
	}
};

#endif PARSER_H