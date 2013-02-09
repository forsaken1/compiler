#include "parser.h"

Parser::Parser(Scanner *_scanner, bool _simple, bool print) {
	scanner = _scanner;
	currentToken = NULL;
	lastToken = NULL;
	top = NULL;
	simple = _simple;
	symStack = new SymTableStack();

	InitTables();
	Parse();
	
	if(print) 
		top->Print(0, true);
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

void Parser::Parse() {
	Next();
	if(simple)
		top = Statement();
	else
		top = Program();
}

//--- Parse Expression ----------

Node* Parser::Expression() {
	Node *left = AssignmentExpr();

	if(oper == COMMA) {
		Next();
		Node *right = Expression();
		if(right == NULL)
			throw ParserException(currentToken, "Enumeration without expression after ','");

		return new NodeBinary(left, COMMA, right);
	}
	return left;
}

Node* Parser::AssignmentExpr() {
	Node *left = ConditionalExpr();

	if(assignmentOperator[oper]) {
		TokenValue tv = oper;
		Next();
		Node *right = AssignmentExpr();
		if(right == NULL)
			throw ParserException(currentToken, "Assignment expression without right operand");

		return new NodeBinary(left, tv, right);
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
			throw ParserException(currentToken, "Ternary expression without ':'");
	}
	return condition;
}

Node* Parser::BinaryOperationExpr(int priority) {
	Node *left = NULL, *right = NULL;

	if(priority == 10)
		left = CastExpr();
	else
		left = BinaryOperationExpr(priority + 1);

	return DeleteLeftRecursion(priority, left);
}

Node* Parser::DeleteLeftRecursion(int priority, Node *left) {
	bool condition = false;

	switch(priority) {
		case 0:  condition = oper == OPER_OR; break;
		case 1:  condition = oper == OPER_AND; break;
		case 2:  condition = oper == OPER_BINARY_OR; break;
		case 3:  condition = oper == OPER_EXCLUSIVE_OR; break;
		case 4:  condition = oper == OPER_BINARY_AND; break;
		case 5:  condition = oper == OPER_EQUAL || oper == OPER_NOT_EQUAL; break;
		case 6:  condition = oper == OPER_LESS || oper == OPER_MORE || oper == OPER_LESS_OR_EQUAL || oper == OPER_MORE_OR_EQUAL; break;
		case 7:  condition = oper == OPER_SHIFT_LEFT || oper == OPER_SHIFT_RIGHT; break;
		case 8:  condition = oper == OPER_PLUS || oper == OPER_MINUS; break;
		case 9:  condition = oper == OPER_MULTIPLY || oper == OPER_DIVIDE || oper == OPER_DIVIDE_BY_MOD; break;
		case 10: condition = oper == OPER_XCHANGE; break;
	}
	if(!condition) return left;

	TokenValue tv = oper;
	Next();
	Node *right = NULL;

	if(priority == 10)
		right = CastExpr();
	else
		right = BinaryOperationExpr(priority + 1);

	if(right == NULL)
		throw ParserException(currentToken, "Binary expression without right operand");

	return DeleteLeftRecursion(priority, new NodeBinary(left, tv, right));
}

Node* Parser::CastExpr() {
	if(symStack->TypeAt(text) && lastToken->GetTokenValue() == ROUND_LEFT_BRACKET) {
		Next();
		if(oper == ROUND_RIGHT_BRACKET) {
			Next();
			Node *right = CastExpr();
			if(right == NULL)
				throw ParserException(currentToken, "Cast expression without right operand");

			return new NodeUnary(CAST_CHAR, right);
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
			if(!symStack->TypeAt(text))
				throw ParserException(currentToken, "Undefinite type");

			Next();
			if(oper == ROUND_RIGHT_BRACKET) {
				Next();
				return new NodeUnary(SIZE_OF, new NodeConst(type, type));
			}
			else
				throw ParserException(currentToken, "Sizeof operation without ')'");
		}
		else {
			Next();
			Node *link = UnaryExpr();
			if(link == NULL)
				throw ParserException(currentToken, "Sizeof operation without argument");

			return new NodeUnary(SIZE_OF, link);
		}
	}
	if(oper == OPER_INC || oper == OPER_DEC) {
		TokenValue _oper = oper;
		Next();
		Node *right = UnaryExpr();
		if(right == NULL)
			throw ParserException(currentToken, "No left operand");

		return new NodeUnary(_oper, right);
	}
	if(unaryOperator[oper]) {
		TokenValue _oper = oper;
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
		if( !simple && symStack->FindVar(left->GetName())->GetSymType() != FUNCTION )
			throw SemanticException(currentToken, "Term not a function");

		Next();
		Node *args = ArgumentExprList();

		if(oper != ROUND_RIGHT_BRACKET)
			throw ParserException(currentToken, "Postfix expression without ')'");

		Next();

		SymbolType type = ( (SymFunction*)( symStack->FindVar(left->GetName()) ) )->GetReturnType();

		return new NodeCall(type, left->GetName(), args);
	}
	if(oper == SQUARE_LEFT_BRACKET) {
		if( !simple && symStack->FindVar(left->GetName())->GetSymType() != ARRAY )
			throw SemanticException(currentToken, "Term not an array");

		Next();
		Node *link = Expression();

		if(oper == SQUARE_RIGHT_BRACKET) {
			Next();
			return new NodeBinary(left, ARRAY_INDEX,  link);
		}
		else throw ParserException(currentToken, "No ']'");
	}
	if(oper == OPER_POINT || oper == OPER_ARROW) {
		TokenValue _oper = oper;
		Next();
		Node *right = PrimaryExpr();
		if(right == NULL)
			throw ParserException(currentToken, "No field");

		return new NodeBinary(left, _oper, right);
	}
	if(oper == OPER_INC || oper == OPER_DEC) {
		TokenValue _oper = oper;
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
		if(right == NULL)
			throw ParserException(currentToken, "Argument list without expression after ','");

		return new NodeStmt("(,)", left, right);
	}
	return left;
}

Node* Parser::PrimaryExpr() {
	if(currentToken->GetTokenType() == IDENTIFIER) {
		string _text = text;

		Next();
		if(!simple) {
			if( !symStack->VarAt(_text) )
				throw SemanticException(currentToken, "Undeclared identifier");

			if( symStack->TypeAt(_text) )
				throw SemanticException(currentToken, "Using type as identifier");
		}
		return new NodeVar(_text);
	}
	if(currentToken->GetTokenType() == CONST_INTEGER ||
	   currentToken->GetTokenType() == CONST_REAL ||
	   currentToken->GetTokenType() == CONST_CHAR ||
	   currentToken->GetTokenType() == CONST_STRING) {
		   
		string id = GetRandomId("const_");
		if(currentToken->GetTokenType() == CONST_CHAR ||
		   currentToken->GetTokenType() == CONST_STRING) {
			symStack->GetTopTable()->AddConst(new SymConst(id, text));
		}
		Next();
		return new NodeConst(id, lastToken->GetText());
	}

	Node *expr = NULL;

	if(oper == ROUND_LEFT_BRACKET) {
		Next();
		expr = Expression();

		if(expr == NULL)
			throw ParserException(currentToken, "Expression expected");

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
		if(simple)
			return StatementList();

		return DeclarationList();
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
	if(link == NULL) link = PauseStmt();
	if(link == NULL) link = LabelStmt();

	return link;
}

Node* Parser::PauseStmt() {
	if(oper == KEYWORD_PAUSE) {
		Next();
		if(oper != SEMICOLON)
			throw ParserException(currentToken, "Pause statement without ';'");

		Next();
		return new NodePause();
	}
	return NULL;
}

Node* Parser::PrintStmt() {
	if(oper == KEYWORD_PRINT || oper == KEYWORD_SCAN) {
		TokenValue _oper = oper;
		Next();
		if(oper != ROUND_LEFT_BRACKET)
			throw ParserException(currentToken, "Print statement without '('");

		Next();
		if(currentToken->GetTokenType() != CONST_STRING)
			throw ParserException(currentToken, "Print statement without format");

		string format = text;
		string random = GetRandomId("const_");

		symStack->GetTopTable()->AddConst(new SymConst(random, format));

		Next();
		if(oper == ROUND_RIGHT_BRACKET) {
			Next();
			if(oper != SEMICOLON) 
				throw ParserException(currentToken, "Print statement without ';'");

			Next();
			return new NodePrint(_oper, new NodeConst(random, format), NULL);
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
		return new NodePrint(_oper, new NodeConst(random, format), expr);
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
					Node *node = new NodeBinary(name, OPER_ASSIGN, Initialiser());

					if(oper == SEMICOLON) {
						Next();
						return node;
					}
					if(oper == COMMA) {
						Next();
						return new NodeStmt("(,)", node, InitDeclaratorList(type));
					}
				}
				if(oper == COMMA) {
					Next();
					return new NodeStmt("(,)", name, InitDeclaratorList(type));
				}
			}
			SymTable *table = new SymTable();
			symStack->Push(table);

			Next();
			Node* args = FunctionArgumentList();

			if(oper != ROUND_RIGHT_BRACKET)
				throw ParserException(currentToken, "Function definition without ')'");

			Next();
			Node *stmt = CompoundStmt();

			if(stmt == NULL)
				throw ParserException(currentToken, "Function definition without statement");

			symStack->Pop();
			symStack->GetTopTable()->AddVar(new SymFunction(type, name->GetName()));

			return new NodeFunc(table, type, name, args, stmt);
		}
	}
	return NULL;
}

Node* Parser::FunctionArgumentList() {
	Node *first = FunctionArgument();

	if(first != NULL) {
		if(oper != COMMA)
			return first;

		Next();
		Node *second = FunctionArgumentList();

		return new NodeStmt("(,)", second, first);
	}
	return NULL;
}

Node* Parser::FunctionArgument() {
	Symbol *type = TypeSpec();

	if(type != NULL) {
		Node *init = InitDeclarator(type);
		if(init == NULL)
			throw ParserException(currentToken, "Function argument list without arguments");

		return new NodeLocalVar(init->GetName());
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

Node* Parser::LabelStmt() {
	if(oper == KEYWORD_LABEL) {
		Node* node = NULL;

		Next();
		if(currentToken->GetTokenType() == IDENTIFIER)
			node = new NodeLabel(text);
		else
			throw ParserException(currentToken, "Label statement without identifier");

		if(!symStack->VarAt(text))
			symStack->AddVar(new SymLabel(text));

		Next();
		if(oper != OPER_COLON)
			throw ParserException(currentToken, "Label statement without ':'");

		Next();
		return node;
	}
	return NULL;
}

Node* Parser::CompoundStmt() {
	if(oper == FIGURE_LEFT_BRACKET) {
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

		return new NodeSafeStmt(link);
	}
	else 
		if(link != NULL)
			throw ParserException(currentToken, "Expression without ';'");

	return NULL;
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
		if(currentToken->GetTokenType() != IDENTIFIER)
			throw ParserException(currentToken, "Jump statement without label");

		Node *ident = new NodeVar(text);

		if(!symStack->VarAt(text))
			symStack->AddVar(new SymLabel(text));

		Next();
		if(oper != SEMICOLON)
			throw ParserException(currentToken, "Jump statement without ';'");

		Next();
		return new NodeJumpStmt(KEYWORD_GOTO, ident);
	}
	if(oper == KEYWORD_CONTINUE || oper == KEYWORD_BREAK) {
		TokenValue _oper = oper;
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
		return new NodeJumpStmt(KEYWORD_RETURN, expr);
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
		symStack->AddType(new SymTypeStruct(str->GetName()));
	}
	if(str == NULL && symStack->TypeAt(text)) {
		string _oper = text;
		Next();
		return symStack->FindType(_oper);
	}
	return NULL;
}

Node* Parser::InitDeclaratorList(Symbol *type) {
	Node *first = InitDeclarator(type);

	if(oper == COMMA) {
		Next();
		Node *second = InitDeclaratorList(type);

		if(second == NULL)
			throw ParserException(currentToken, "Expected expression after ','");

		return new NodeSafeStmt(first, second);
	}
	if(first != NULL)
		return new NodeSafeStmt(first);

	return NULL;
}

Node* Parser::InitDeclarator(Symbol *type) {
	Node *decl = Declarator(type);
	
	if(oper == OPER_ASSIGN) {
		Next();
		Node *init = Initialiser();

		if(init == NULL)
			throw ParserException(currentToken, "Init declarator without initialiser");

		return new NodeBinary(decl, OPER_ASSIGN, init);
	}
	return decl;
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
			return new NodeBinary(init, COMMA, InitialiserList());
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
	if(currentToken->GetTokenType() == IDENTIFIER) {
		if( !simple && symStack->VarAt(text) )
			throw SemanticException(currentToken, "Redefinition identifier");

		string _oper = text;
		Next();
		if(oper == SQUARE_LEFT_BRACKET) {
			Next();
			int length = 0;

			if(currentToken->GetTokenType() == CONST_INTEGER) {
				length = atoi(text.c_str());
				Next();
			}
			else
				throw ParserException(currentToken, "Array declaration without length");

			if(oper != SQUARE_RIGHT_BRACKET)
				throw ParserException(currentToken, "Array declaration without ']'");

			symStack->AddVar(new SymTypeArray(type, _oper, length));
			Next();
			return new NodeVar(_oper);
		}
		if(oper != ROUND_LEFT_BRACKET)
			symStack->AddVar(new SymVar(type, _oper));  //symtable add

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

		if(currentToken->GetTokenType() == IDENTIFIER) {
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
	assignmentOperator[DEFAULT] = false;
	assignmentOperator[OPER_ASSIGN] = true;
	assignmentOperator[OPER_MULTIPLY_ASSIGN] = true;
	assignmentOperator[OPER_DIVIDE_ASSIGN] = true;
	assignmentOperator[OPER_DIVIDE_BY_MOD_ASSIGN] = true;
	assignmentOperator[OPER_PLUS_ASSIGN] = true;
	assignmentOperator[OPER_MINUS_ASSIGN] = true;
	assignmentOperator[OPER_BINARY_AND_ASSIGN] = true;
	assignmentOperator[OPER_EXCLUSIVE_OR_ASSIGN] = true;
	assignmentOperator[OPER_BINARY_OR_ASSIGN] = true;
	
	unaryOperator[DEFAULT] = false;
	unaryOperator[OPER_BINARY_AND] = true;
	unaryOperator[OPER_MULTIPLY] = true;
	unaryOperator[OPER_PLUS] = true;
	unaryOperator[OPER_MINUS] = true;
	unaryOperator[OPER_BINARY_NOT] = true;
	unaryOperator[OPER_NOT] = true;

	symStack->GetTopTable()->AddType(new SymTypeVoid());
	symStack->GetTopTable()->AddType(new SymTypeChar());
	symStack->GetTopTable()->AddType(new SymTypeInteger());
	symStack->GetTopTable()->AddType(new SymTypeFloat());
}