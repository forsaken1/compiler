#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "node.h"
#include "sym_table.h"

class Parser {
	Token *currentToken, *lastToken;
	Node *top;
	Scanner *scanner;
	string oper;
	map<string, bool> assignmentOperator, unaryOperator, typeName;
	
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
	Node* StatementList();
	Node* CompoundStmt();
	Node* ExpressionStmt();
	Node* DeclarationStmt();
	Node* SelectionStmt();
	Node* IterationStmt();
	Node* FunctionDefinitionStmt();
	Node* JumpStmt();

	Symbol* Declaration();
	Symbol* DeclarationSpec();
	Symbol* TypeSpec();
	Symbol* InitDeclarationList();
	Symbol* InitDeclarator();
	Symbol* Declarator();
	Symbol* DirectDeclarator();
	Symbol* Pointer();
	
	void Next();
	void InitHashes();

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