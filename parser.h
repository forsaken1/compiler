#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "node.h"

class Parser {
	Token *currentToken, *lastToken;
	Node *top;
	Scanner *scanner;
	SymTable *globalType;
	SymTableStack *symStack;
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
	Node* ArgumentExprList();

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
	Parser(Scanner*);
};

class ParserException {
	string msg;
	Token *tk;

public:
	ParserException(Token *_tk, string _msg) {
		msg = _msg;
		tk = _tk;
	}

	string GetMessage() {
		return "Syntax error: " + msg /* + " on line " + string(1, tk->GetLine() + '0') + " in position " + string(1, tk->GetPos() + '0')*/;
	}
};

#endif PARSER_H