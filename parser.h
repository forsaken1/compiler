#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "node.h"

class Parser {
	Token *currentToken, *lastToken;
	Node *top;
	Scanner *scanner;
	SymTable *globalVar, *globalType;
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
	Node* FunctionArgumentList();
	Node* FunctionArgument();
	Node* JumpStmt();

	Node* Declaration();
	Node* DeclarationList();
	Node* InitDeclarator(Symbol*);
	Node* InitDeclaratorList(Symbol*);
	Node* Declarator(Symbol*);
	Node* DirectDeclarator(Symbol*);
	Node* Initialiser();
	Node* InitialiserList();
	Node* Pointer();

	Node* StructSpec();
	Node* StructDeclarationList();
	Node* StructDeclaration();
	Node* SpecList();
	Node* StructDeclaratorList();
	Node* StructDeclarator();

	Symbol* TypeSpec();
	
	void Next();
	void InitTables();

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