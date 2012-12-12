#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "node.h"

class Parser {
	Token *currentToken, *lastToken;
	Node *top;
	Scanner *scanner;
	string oper;
	map<string, bool> assignmentOperator, unaryOperator;
	
	void Parse();

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
	void InitHashes();

public:
	Parser(Scanner*);
};

enum ParserExceptionType {
	SUCCESS,
	ERROR
};

class ParserException {
	string msg;
	ParserExceptionType type;

public:
	ParserException(ParserExceptionType _type, string _msg) {
		msg = _msg;
		type = _type;
	}

	string GetMessage() {
		return (type == SUCCESS ? "Build success. " : "Syntax error: ") + msg;
	}
};

#endif PARSER_H