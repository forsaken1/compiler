#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "node.h"

class Parser {
	Token *currentToken, *lastToken;
	Node *top;
	Scanner *scanner;
	SymTableStack *symStack;
	TokenValue oper;
	string text;
	map<string, bool> assignmentOperator, unaryOperator;
	bool simple; // for testing parser without semantics
	
	void Parse();

	Node* Expression();
	Node* AssignmentExpr();
	Node* ConditionalExpr();
	Node* BinaryOperationExpr(int);
	Node* CastExpr();
	Node* UnaryExpr();
	Node* PostfixExpr();
	Node* PrimaryExpr();
	Node* ArgumentExprList();
	
	Node* Program();
	Node* ExternalDecl();
	Node* Statement();
	Node* PrintStmt();
	Node* StatementList();
	Node* CompoundStmt();
	Node* ExpressionStmt();
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
	Node* StructDeclaratorList(Symbol*);
	Node* StructDeclarator(Symbol*);

	Symbol* TypeSpec();
	
	void Next();
	void InitTables();

public:
	Parser(Scanner*, bool, bool);
	Node* GetTop();
	SymTableStack* GetSymStack();
};

#endif PARSER_H