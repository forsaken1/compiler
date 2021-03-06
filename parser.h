#ifndef PARSER_H
#define PARSER_H

#include "node.h"

class Parser {
	Token *currentToken, *lastToken;
	Node *top;
	Scanner *scanner;
	SymTableStack *symStack;
	TokenValue oper;
	string text;
	map<TokenValue, bool> assignmentOperator, unaryOperator;
	bool simple; // for testing parser without semantics
	
	void Parse();
	
	Node* Expression();
	Node* AssignmentExpr();
	Node* ConditionalExpr();
	Node* BinaryOperationExpr(int);
	Node* DeleteLeftRecursion(int, Node*);
	Node* CastExpr();
	Node* UnaryExpr();
	Node* PostfixExpr();
	Node* PrimaryExpr();
	Node* ArgumentExprList();
	
	Node* Program();
	Node* ExternalDecl();
	Node* Statement();
	Node* PrintStmt();
	Node* PauseStmt();
	Node* StatementList();
	Node* CompoundStmt();
	Node* ExpressionStmt();
	Node* SelectionStmt();
	Node* IterationStmt();
	Node* FunctionDefinitionStmt();
	Node* FunctionArgumentList();
	Node* FunctionArgument();
	Node* JumpStmt();
	Node* LabelStmt();

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