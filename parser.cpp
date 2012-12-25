#include "parser.h"
#include <stdlib.h>
#include <time.h>

Parser::Parser(Scanner *_scanner, bool _simple, bool _print) {
	scanner = _scanner;
	currentToken = NULL;
	lastToken = NULL;
	top = NULL;
	simple = _simple;
	print = _print;
	symStack = new SymTableStack();
	InitTables();

	Parse();
}

Node* Parser::GetTop() {
	return top;
}

SymTableStack* Parser::GetSymStack() {
	return symStack;
}

void Parser::Next() {
	scanner->Next();
	delete lastToken;
	lastToken = currentToken;
	currentToken = scanner->GetToken();
	oper = currentToken->GetTokenValue();
	text = currentToken->GetText();
}

//--- Parse Expression ----------

void Parser::Parse() {
	Next();
	if(simple)
		top = Statement();
	else
		top = Program();
	
	if(print) 
		top->Print(0, true);
}

Node* Parser::Expression() {
	Node *left = AssignmentExpr();

	if(oper == COMMA) {
		string _text = text;
		Next();
		Node *right = Expression();
		if(right == NULL)
			throw ParserException(currentToken, "Enumeration without expression after ','");

		return new NodeBinary(left, _text, right);
	}
	return left;
}

Node* Parser::AssignmentExpr() {
	Node *left = ConditionalExpr();

	if(assignmentOperator[text]) {
		string _text = text;
		Next();
		Node *right = AssignmentExpr();
		if(right == NULL)
			throw ParserException(currentToken, "Assignment expression without left operand");

		return new NodeBinary(left, _text, right);
	}
	return left;
}

Node* Parser::ConditionalExpr() {
	Node *condition = BinaryOperationExpr(0);

	if(oper == OPER_QUESTION) {
		Next();
		Node *cond_true = Expression();
		if(cond_true == NULL)
			throw ParserException(currentToken, "Ternary expression without true-condition");

		if(oper == OPER_COLON) {
			Next();
			Node *cond_false = ConditionalExpr();
			if(cond_false == NULL)
				throw ParserException(currentToken, "Ternary expression without false-condition");

			return new NodeSelectionStmt(condition, cond_true, cond_false);
		}
		else
			throw ParserException(currentToken, "Ternary expression without false-condition");
	}
	return condition;
}

Node* Parser::BinaryOperationExpr(int priority) {
	Node *left = NULL;

	if(priority == 9)
		left = CastExpr();
	else
		left = BinaryOperationExpr(priority + 1);

	bool condition = false;

	switch(priority) {
		case 0: condition = oper == OPER_OR; break;
		case 1: condition = oper == OPER_AND; break;
		case 2: condition = oper == OPER_BINARY_OR; break;
		case 3: condition = oper == OPER_EXCLUSIVE_OR; break;
		case 4: condition = oper == OPER_BINARY_AND; break;
		case 5: condition = oper == OPER_EQUAL || oper == OPER_NOT_EQUAL; break;
		case 6: condition = oper == OPER_LESS || oper == OPER_MORE || oper == OPER_LESS_OR_EQUAL || oper == OPER_MORE_OR_EQUAL; break;
		case 7: condition = oper == OPER_SHIFT_LEFT || oper == OPER_SHIFT_RIGHT; break;
		case 8: condition = oper == OPER_PLUS || oper == OPER_MINUS; break;
		case 9: condition = oper == OPER_MULTIPLY || oper == OPER_DIVIDE || oper == OPER_DIVIDE_BY_MOD; break;
	}
	if(condition) {
		string _oper = text;
		Next();
		Node *right = BinaryOperationExpr(priority);
		if(right == NULL)
			throw ParserException(currentToken, "Binary expression without left operand");

		return new NodeBinary(left, _oper, right);
	}
	return left;
}

Node* Parser::CastExpr() {
	if(symStack->GetTopTable()->TypeAt(text) && lastToken->GetTokenValue() == ROUND_LEFT_BRACKET) {
		string _oper = text;
		Next();
		if(oper == ROUND_RIGHT_BRACKET) {
			Next();
			Node *right = CastExpr();
			if(right == NULL)
				throw ParserException(currentToken, "Cast expression without left operand");

			return new NodeUnary(_oper, right);
		}
		else
			throw ParserException(currentToken, "Cast expression without ')'");
	}
	return UnaryExpr();
}

Node* Parser::UnaryExpr() {
	if(oper == KEYWORD_SIZEOF) {
		Next();
		if(oper == ROUND_LEFT_BRACKET) {
			Next();
			string type = text;
			if(symStack->GetTopTable()->TypeAt(text)) {
				Next();
			}
			else/////////////////!!!!!!!!!!!!!!
				ParserException(currentToken, "Undefinite type");

			if(oper == ROUND_RIGHT_BRACKET) {
				Next();
				return new NodeUnary("sizeof", new NodeConst(type));
			}
		}
		else {
			Next();
			Node *link = UnaryExpr();
			return new NodeUnary("sizeof", link);
		}
	}
	if(oper == OPER_INC || oper == OPER_DEC) {
		string _oper = text;
		Next();
		Node *right = UnaryExpr();
		if(right == NULL)
			throw ParserException(currentToken, "No left operand");

		return new NodeUnary(_oper, right);
	}
	if(unaryOperator[text]) {
		string _oper = text;
		Next();
		Node *right = CastExpr();
		if(right == NULL)
			throw ParserException(currentToken, "No left operand");

		return new NodeUnary(_oper, right);
	}	
	return PostfixExpr();
}

Node* Parser::PostfixExpr() {
	Node *left = PrimaryExpr();

	if(oper == ROUND_LEFT_BRACKET) {
		Next();
		Node *args = ArgumentExprList();

		if(oper != ROUND_RIGHT_BRACKET)
			throw ParserException(currentToken, "Postfix expression without ')'");

		Next();
		if(args == NULL)
			return new NodeUnary("()", left);

		return new NodeBinary(left, "()", args);
	}
	if(oper == SQUARE_LEFT_BRACKET) {
		Next();
		Node *link = Expression();

		if(oper == SQUARE_RIGHT_BRACKET) {
			Next();
			return new NodeBinary(left, "[]",  link);
		}
		else throw ParserException(currentToken, "No ']'");
	}
	if(oper == OPER_POINT || oper == OPER_ARROW) {
		string _oper = text;
		Next();
		Node *right = PrimaryExpr();
		if(right == NULL)
			throw ParserException(currentToken, "No field");

		return new NodeBinary(left, _oper, right);
	}
	if(oper == OPER_INC || oper == OPER_DEC) {
		string _oper = text;
		Next();
		return new NodeUnary(_oper, left);
	}
	return left;
}

Node* Parser::ArgumentExprList() {
	Node *left = AssignmentExpr();

	if(oper == COMMA) {
		Next();
		Node *right = ArgumentExprList();
		return new NodeBinary(left, ",", right);
	}
	return left;
}

string GetRandomId() {
	string random = "";
	srand ( time(NULL) );

	for(int j = 0; j < 10; j++)
		random += string(1, (rand() % 10) + 48);

	return random;
}

Node* Parser::PrimaryExpr() {
	if(currentToken->GetType() == IDENTIFIER) {
		if( !simple && !symStack->GetTopTable()->VarAt(text) )
			throw SemanticException(currentToken, "Undeclared identifier");

		Next();
		return new NodeVar(lastToken->GetText());
	}

	if(currentToken->GetType() == CONST_INTEGER ||
	   currentToken->GetType() == CONST_REAL ||
	   currentToken->GetType() == CONST_CHAR ||
	   currentToken->GetType() == CONST_STRING) {
		symStack->GetTopTable()->AddConst(new SymConst(GetRandomId(), text));
		Next();
		return new NodeConst(lastToken->GetText());
	}

	Node *expr = NULL;

	if(oper == ROUND_LEFT_BRACKET) {
		Next();
		expr = Expression();

		if(oper == ROUND_RIGHT_BRACKET) {
			Next();
			return expr;
		}
		return expr;
	}
	return NULL;
}

//--- Parse Statement ----------

Node* Parser::Program() {
	Node* first = ExternalDecl();

	if(first != NULL) {
		Node* second = Program();
		if(second == NULL)
			return first;

		return new NodeStmt("<external>", first, second);
	}
	return NULL;
}

Node* Parser::ExternalDecl() {
	Node* def = FunctionDefinitionStmt();
	
	if(def == NULL) {
		return Statement();
	}
	return def;
}

Node* Parser::Statement() {
	Node *link = NULL;
	
	if(simple)
		link = FunctionDefinitionStmt();

	if(link == NULL) link = CompoundStmt();
	if(link == NULL) link = ExpressionStmt();
	if(link == NULL) link = DeclarationList();
	if(link == NULL) link = SelectionStmt();
	if(link == NULL) link = IterationStmt();
	if(link == NULL) link = JumpStmt();
	if(link == NULL) link = PrintStmt();

	return link;
}

Node* Parser::PrintStmt() {
	if(oper == KEYWORD_PRINT) {
		Next();
		if(oper != ROUND_LEFT_BRACKET)
			throw ParserException(currentToken, "Print statement without '('");

		Next();
		if(currentToken->GetType() != CONST_STRING)
			throw ParserException(currentToken, "Print statement without format");

		string format = text;
		string random = GetRandomId();

		symStack->GetTopTable()->AddConst(new SymConst(random, format));

		Next();
		if(oper == ROUND_RIGHT_BRACKET) {
			Next();
			if(oper != SEMICOLON) 
				throw ParserException(currentToken, "Print statement without ';'");

			Next();
			return new NodePrint(new SymConst(random, format), NULL);
		}
		if(oper != COMMA)
			throw ParserException(currentToken, "Print statement without ','");

		Next();
		Node *expr = Expression();

		if(expr == NULL)
			throw ParserException(currentToken, "Print statement without expression");

		if(oper != ROUND_RIGHT_BRACKET)
			throw ParserException(currentToken, "Print statement without ')'");

		Next();
		if(oper != SEMICOLON)
			throw ParserException(currentToken, "Print statement without ';'");

		Next();
		return new NodePrint(new SymConst(random, format), expr);
	}
	return NULL;
}

Node* Parser::FunctionDefinitionStmt() {
	Symbol *type = TypeSpec();

	if(type != NULL) {
		Node *name = Declarator(type);

		if(name != NULL) {
			if(oper != ROUND_LEFT_BRACKET) {
				if(oper == SEMICOLON) {
					Next();
					return name;
				}
				if(oper == OPER_ASSIGN) {
					Next();
					Node *node = new NodeBinary(name, "=", Initialiser());
					if(oper == SEMICOLON) {
						Next();
						return node;
					}
					if(oper == COMMA) {
						Next();
						return new NodeBinary(node, ",", InitDeclaratorList(type));
					}
				}
				if(oper == COMMA) {
					Next();
					return new NodeBinary(name, ",", InitDeclaratorList(type));
				}
			}
			Next();
			Node* args = FunctionArgumentList();

			if(oper != ROUND_RIGHT_BRACKET)
				throw ParserException(currentToken, "Function definition without ')'");

			Next();
			Node *stmt = CompoundStmt();

			if(stmt == NULL)
				throw ParserException(currentToken, "Function definition without statement");

			return new NodeFunc(type, name, args, stmt);
		}
	}
	return NULL;
}

Node* Parser::FunctionArgumentList() {
	Node *args = FunctionArgument();

	if(args != NULL) {
		if(oper != COMMA)
			return args;

		Next();
		Node *link = FunctionArgumentList();

		return new NodeBinary(args, ",", link);
	}
	return NULL;
}

Node* Parser::FunctionArgument() {
	Symbol *type = TypeSpec();

	if(type != NULL) {
		Node *init = InitDeclarator(type);
		if(init == NULL)
			throw ParserException(currentToken, "Function argument list without arguments");

		return init;
	}
	return NULL;
}

Node* Parser::StatementList() {
	Node *first = Statement();

	if(first != NULL) {
		Node *second = StatementList();
		if(second == NULL)
			return first;

		return new NodeStmt("(stmt)", first, second);
	}
	return NULL;
}

Node* Parser::CompoundStmt() {
	if(oper == FIGURE_LEFT_BRACKET) {
		//symStack->Push(new SymTable()); //дописать поиск в SymStack!!!!!

		Next();
		Node *decl = DeclarationList();
		Node *stmt = StatementList();

		if(oper != FIGURE_RIGHT_BRACKET)
			throw ParserException(currentToken, "Compound statement without '}'");
		else
			Next();
		
		return new NodeStmt("{stmt}", decl, stmt);
	}
	return NULL;
}

Node* Parser::ExpressionStmt() {
	Node *link = Expression();

	if(oper == SEMICOLON) {
		Next();
		if(link == NULL)
			return new NodeStmt("(stmt)", NULL, NULL);
	}
	else 
		if(link != NULL)
			throw ParserException(currentToken, "Expression without ';'");

	return link;
}

Node* Parser::SelectionStmt() {
	if(oper == KEYWORD_IF) {
		Next();
		if(oper != ROUND_LEFT_BRACKET)
			throw ParserException(currentToken, "Selection statement without '('");
		
		Next();
		Node *expr = Expression();

		if(expr == NULL)
			throw ParserException(currentToken, "Selection statement without condition");

		if(oper != ROUND_RIGHT_BRACKET)
			throw ParserException(currentToken, "Selection statement without ')'");
		
		Next();
		Node *ifTrue = Statement();

		if(oper == KEYWORD_ELSE) {
			Next();
			Node *ifFalse = Statement();
			return new NodeSelectionStmt(expr, ifTrue, ifFalse);
		}
		return new NodeSelectionStmt(expr, ifTrue, NULL);
	}
	return NULL;
}

Node* Parser::IterationStmt() {
	if(oper == KEYWORD_FOR) {
		Next();
		if(oper != ROUND_LEFT_BRACKET)
			throw ParserException(currentToken, "Iteration statement without '('");

		Next();
		Node *forInit = Expression();

		if(oper != SEMICOLON)
			throw ParserException(currentToken, "Iteration statement without separator in condition");

		Next();
		Node *forCond = Expression();

		if(oper != SEMICOLON)
			throw ParserException(currentToken, "Iteration statement without separator in condition");

		Next();
		Node *forIter = Expression();

		if(oper != ROUND_RIGHT_BRACKET)
			throw ParserException(currentToken, "Iteration statement without ')'");

		Next();
		Node *stmt = Statement();

		return new NodeIterationFor(forInit, forCond, forIter, stmt);
	}
	if(oper == KEYWORD_WHILE) {
		Next();
		if(oper != ROUND_LEFT_BRACKET)
			throw ParserException(currentToken, "Iteration statement without '('");

		Next();
		Node *expr = Expression();

		if(expr == NULL)
			throw ParserException(currentToken, "Iteration statement without expression");

		if(oper != ROUND_RIGHT_BRACKET)
			throw ParserException(currentToken, "Iteration statement without ')'");

		Next();
		Node *stmt = Statement();

		return new NodeIterationWhile(expr, stmt);
	}
	if(oper == KEYWORD_DO) {
		Next();
		Node *stmt = Statement();
		
		if(oper == KEYWORD_WHILE) {
			Next();
			if(oper != ROUND_LEFT_BRACKET)
				throw ParserException(currentToken, "Iteration statement without '('");

			Next();
			Node *expr = Expression();

			if(expr == NULL)
				throw ParserException(currentToken, "Iteration statement without expression");

			if(oper != ROUND_RIGHT_BRACKET)
				throw ParserException(currentToken, "Iteration statement without ')'");

			Next();
			return new NodeIterationDo(expr, stmt);
		}
		else 
			throw ParserException(currentToken, "Iteration statement without condition");
	}
	return NULL;
}

Node* Parser::JumpStmt() {
	if(oper == KEYWORD_GOTO) {
		Next();
		Node *ident = PrimaryExpr();

		if(ident == NULL)
			throw ParserException(currentToken, "Jump statement without label");

		if(oper != SEMICOLON)
			throw ParserException(currentToken, "Jump statement without ';'");

		Next();
		return new NodeJumpStmt("goto", ident);
	}
	if(oper == KEYWORD_CONTINUE || oper == KEYWORD_BREAK) {
		string _oper = text;
		Next();
		if(oper != SEMICOLON)
			throw ParserException(currentToken, "Jump statement without ';'");

		Next();
		return new NodeJumpStmt(_oper, NULL);
	}
	if(oper == KEYWORD_RETURN) {
		Next();
		Node *expr = Expression();

		if(oper != SEMICOLON)
			throw ParserException(currentToken, "Jump statement without ';'");

		Next();
		return new NodeJumpStmt("return", expr);
	}
	return NULL;
}

//--- Parse Declaration ---

Node* Parser::DeclarationList() {
	Node *decl = Declaration();

	if(decl != NULL) {
		Node *link = DeclarationList();

		if(link == NULL) {
			return decl;
		}
		return new NodeStmt("<decl>", decl, link);
	}
	return NULL;
}

Node* Parser::Declaration() {
	Symbol *type = TypeSpec();

	if(type != NULL) {
		Node *init = InitDeclaratorList(type);
		if(oper != SEMICOLON)
			throw ParserException(currentToken, "Init declarator list without ';'");
		else
			Next();

		return init;
	}
	return NULL;
}

Symbol* Parser::TypeSpec() {
	Node *str = StructSpec();

	if(str != NULL) {
		//добавление struct в SymTable
	}
	if(str == NULL && symStack->GetTopTable()->TypeAt(text)) {
		string _oper = text;
		Next();
		return symStack->GetTopTable()->FindType(_oper);
	}
	return NULL;
}

Node* Parser::InitDeclaratorList(Symbol *type) {
	Node *first = InitDeclarator(type);

	if(first != NULL) {
		if(oper != COMMA)
			return first;

		Next();
		Node *second = InitDeclaratorList(type);
		if(second == NULL) {
			return first;
		}
		return new NodeBinary(first, ",", second);
	}
	return NULL;
}

Node* Parser::InitDeclarator(Symbol *type) {
	Node *decl = Declarator(type);
	
	if(decl != NULL) {
		if(oper != OPER_ASSIGN)
			return decl;

		Next();
		Node *init = Initialiser();
		if(init == NULL)
			throw ParserException(currentToken, "Init declarator without initialiser");

		return new NodeBinary(decl, "=", init);
	}
	return NULL;
}

Node* Parser::Initialiser() {
	if(oper == FIGURE_LEFT_BRACKET) {
		Next();
		Node *init = InitialiserList();
		
		if(oper != FIGURE_RIGHT_BRACKET)
			throw ParserException(currentToken, "Initialiser list without '}'");
		
		Next();
		return init;
	}
	else {
		return AssignmentExpr();
	}
	return NULL;
}

Node* Parser::InitialiserList() {
	Node *init = Initialiser();

	if(init != NULL) {
		if(oper == COMMA) {
			Next();
			return new NodeBinary(init, ",", InitialiserList());
		}
		else 
			return init;
	}
	return NULL;
}

Node* Parser::Declarator(Symbol *type) {
	Node *pointer = Pointer();
	Node *decl = DirectDeclarator(type);

	return decl;
}

Node* Parser::DirectDeclarator(Symbol *type) {
	if(currentToken->GetType() == IDENTIFIER) {
		if( !simple && symStack->GetTopTable()->VarAt(text) )
			throw ParserException(currentToken, "Redefinition identifier");

		symStack->GetTopTable()->AddVar(new SymVar(type, text));  //symtable add

		string _oper = text;
		Next();
		if(oper == SQUARE_LEFT_BRACKET) {
			Next();
			Node *link = ConditionalExpr();
			if(oper != SQUARE_RIGHT_BRACKET)
				throw ParserException(currentToken, "Array declaration without ']'");

			Next();
		}
		return new NodeVar(_oper);
	}
	return NULL;
}

Node* Parser::Pointer() {
	if(oper == OPER_MULTIPLY) {
		Next();
		return Pointer();
	}
	return NULL;
}

//--- Struct Declaration ---

Node* Parser::StructSpec() {
	if(oper == KEYWORD_STRUCT) {
		string ident = "";
		Next();

		if(currentToken->GetType() == IDENTIFIER) {
			ident = oper;
			Next();
		}
		if(oper == FIGURE_LEFT_BRACKET) {
			Next();
			Node *decl = StructDeclarationList();

			if(decl == NULL)
				throw ParserException(currentToken, "Struct without field list");

			if(oper != FIGURE_RIGHT_BRACKET)
				throw ParserException(currentToken, "Struct without '}'");

			Next();
			return new NodeStruct(ident, decl);
		}
		return new NodeStruct(ident, NULL);
	}
	return NULL;
}

Node* Parser::StructDeclarationList() {
	Node *first = StructDeclaration();

	if(first != NULL) {
		Node *second = StructDeclarationList();

		if(second == NULL)
			return first;
		else
			return new NodeStmt("<field>", first, second);
	}
	return NULL;
}

Node* Parser::StructDeclaration() {
	Symbol *type = TypeSpec();

	if(type != NULL) {
		Node *decl = StructDeclaratorList(type);
		
		if(oper == SEMICOLON)
			Next();
		else
			throw ParserException(currentToken, "Struct declaration without ';'");

		return decl;
	}
	return NULL;
}

Node* Parser::StructDeclaratorList(Symbol *type) {
	Node *decl = StructDeclarator(type);

	if(decl != NULL) {
		if(oper == COMMA) {
			Next();
			return new NodeStmt("<field>", decl, StructDeclarationList());
		}
		return decl;
	}
	return NULL;
}

Node* Parser::StructDeclarator(Symbol *type) {
	Node *decl = Declarator(type); 

	if(decl == NULL)
		return ConditionalExpr();

	return decl;
}

//--- Init Hashes ---

void Parser::InitTables() {
	assignmentOperator["="] = true;
	assignmentOperator["*="] = true;
	assignmentOperator["/="] = true;
	assignmentOperator["%="] = true;
	assignmentOperator["+="] = true;
	assignmentOperator["-="] = true;
	assignmentOperator["&="] = true;
	assignmentOperator["^="] = true;
	assignmentOperator["|="] = true;

	unaryOperator["&"] = true;
	unaryOperator["*"] = true;
	unaryOperator["+"] = true;
	unaryOperator["-"] = true;
	unaryOperator["~"] = true;
	unaryOperator["!"] = true;

	symStack->GetTopTable()->AddType(new SymTypeVoid());
	symStack->GetTopTable()->AddType(new SymTypeChar());
	symStack->GetTopTable()->AddType(new SymTypeInteger());
	symStack->GetTopTable()->AddType(new SymTypeFloat());
}