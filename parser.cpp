#include "parser.h"

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
	oper = currentToken->GetText();
}

//--- Parse Expression ----------

void Parser::Parse() {
	try {
		Next();
		if(simple)
			top = Statement();
		else
			top = Program();
		
		if(print) 
			top->Print(0, true);
	}
	catch(Exception &e) {
		cout << e.GetMessage() << endl;
	}
}

Node* Parser::Expression() {
	Node *left = AssignmentExpr();

	if(oper == ",") {
		string _oper = oper;
		Next();
		Node *right = Expression();
		if(right == NULL)
			throw ParserException(currentToken, "Enumeration without expression after ','");

		return new NodeBinary(left, _oper, right);
	}
	return left;
}

Node* Parser::AssignmentExpr() {
	Node *left = ConditionalExpr();

	if(assignmentOperator[oper]) {
		string _oper = oper;
		Next();
		Node *right = AssignmentExpr();
		if(right == NULL)
			throw ParserException(currentToken, "Assignment expression without left operand");

		return new NodeBinary(left, _oper, right);
	}
	return left;
}

Node* Parser::ConditionalExpr() { 
	Node *condition = BinaryOperationExpr(0);

	if(oper == "?") {
		Next();
		Node *cond_true = Expression();
		if(cond_true == NULL)
			throw ParserException(currentToken, "Ternary expression without true-condition");

		if(oper == ":") {
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
		case 0: condition = oper == "||"; break;
		case 1: condition = oper == "&&"; break;
		case 2: condition = oper == "|"; break;
		case 3: condition = oper == "^"; break;
		case 4: condition = oper == "&"; break;
		case 5: condition = oper == "==" || oper == "!="; break;
		case 6: condition = oper == "<"  || oper == ">" || oper == "<=" || oper == ">="; break;
		case 7: condition = oper == ">>" || oper == "<<"; break;
		case 8: condition = oper == "+"  || oper == "-"; break;
		case 9: condition = oper == "*"  || oper == "/" || oper == "%"; break;
	}
	if(condition) {
		string _oper = oper;
		Next();
		Node *right = BinaryOperationExpr(priority);
		if(right == NULL)
			throw ParserException(currentToken, "Binary expression without left operand");

		return new NodeBinary(left, _oper, right);
	}
	return left;
}

Node* Parser::CastExpr() {
	if(symStack->GetTopTable()->TypeAt(oper) && lastToken->GetText() == "(") {
		string _oper = oper;
		Next();
		if(oper == ")") {
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
	if(oper == "sizeof") {
		Next();
		if(oper == "(") {
			Next();
			string type = oper;
			if(symStack->GetTopTable()->TypeAt(oper)) {
				Next();
			}
			if(oper == ")") {
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
	if(oper == "++" || oper == "--") {
		string _oper = oper;
		Next();
		Node *right = UnaryExpr();
		if(right == NULL)
			throw ParserException(currentToken, "No left operand");

		return new NodeUnary(_oper, right);
	}
	if(unaryOperator[oper]) {
		string _oper = oper;
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

	if(oper == "(") {
		Next();
		Node *args = ArgumentExprList();

		if(oper != ")")
			throw ParserException(currentToken, "Postfix expression without ')'");

		Next();
		if(args == NULL)
			return new NodeUnary("()", left);

		return new NodeBinary(left, "()", args);
	}
	if(oper == "[") {
		Next();
		Node *link = Expression();

		if(oper == "]") {
			Next();
			return new NodeBinary(left, "[]",  link);
		}
		else throw ParserException(currentToken, "No ']'");
	}
	if(oper == "." || oper == "->") {
		string _oper = oper;
		Next();
		Node *right = PrimaryExpr();
		if(right == NULL)
			throw ParserException(currentToken, "No field");

		return new NodeBinary(left, _oper, right);
	}
	if(oper == "++" || oper == "--") {
		string _oper = oper;
		Next();
		return new NodeUnary(_oper, left);
	}
	return left;
}

Node* Parser::ArgumentExprList() {
	Node *left = AssignmentExpr();

	if(oper == ",") {
		Next();
		Node *right = ArgumentExprList();
		return new NodeBinary(left, ",", right);
	}
	return left;
}

Node* Parser::PrimaryExpr() {
	if(currentToken->GetType() == IDENTIFIER) {
		if( !simple && !symStack->GetTopTable()->VarAt(oper) )
			throw SemanticException(currentToken, "Undeclared identifier");

		Next();
		return new NodeVar(lastToken->GetText());
	}

	if(currentToken->GetType() == CONST_INTEGER ||
	   currentToken->GetType() == CONST_REAL ||
	   currentToken->GetType() == CONST_CHAR ||
	   currentToken->GetType() == CONST_STRING) {
		symStack->GetTopTable()->AddConst(new Symbol(oper));
		Next();
		return new NodeConst(lastToken->GetText());
	}

	Node *expr = NULL;

	if(oper == "(") {
		Next();
		expr = Expression();

		if(oper == ")") {
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
	if(oper == "print") {
		Next();
		if(oper != "(")
			throw ParserException(currentToken, "Print statement without '('");

		Next();
		Node *expr = Expression();

		if(expr == NULL)
			throw ParserException(currentToken, "Print statement without expression");

		if(oper != ")")
			throw ParserException(currentToken, "Print statement without ')'");

		Next();

		if(oper != ";")
			throw ParserException(currentToken, "Print statement without ';'");

		Next();
		return new NodePrint(expr);
	}
	return NULL;
}

Node* Parser::FunctionDefinitionStmt() {
	Symbol *type = TypeSpec();

	if(type != NULL) {
		Node *name = Declarator(type);

		if(name != NULL) {
			if(oper != "(") {
				if(oper == ";") {
					Next();
					return name;
				}
				if(oper == "=") {
					Next();
					Node *node = new NodeBinary(name, "=", Initialiser());
					if(oper == ";") {
						Next();
					}
					if(oper == ",") {
						Next();
						return new NodeBinary(node, ",", InitDeclaratorList(type));
					}
					else
						return node;
				}
				if(oper == ",") {
					Next();
					return new NodeBinary(name, ",", InitDeclaratorList(type));
				}
			}
			Next();
			Node* args = FunctionArgumentList();

			if(oper != ")")
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
		if(oper != ",")
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
	if(oper == "{") {
		//symStack->Push(new SymTable()); //дописать поиск в SymStack!!!!!

		Next();
		Node *decl = DeclarationList();
		Node *stmt = StatementList();

		if(oper != "}")
			throw ParserException(currentToken, "Compound statement without '}'");
		else
			Next();
		
		return new NodeStmt("{stmt}", decl, stmt);
	}
	return NULL;
}

Node* Parser::ExpressionStmt() {
	Node *link = Expression();

	if(oper == ";") {
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
	if(oper == "if") {
		Next();
		if(oper != "(")
			throw ParserException(currentToken, "Selection statement without '('");
		
		Next();
		Node *expr = Expression();

		if(expr == NULL)
			throw ParserException(currentToken, "Selection statement without condition");

		if(oper != ")")
			throw ParserException(currentToken, "Selection statement without ')'");
		
		Next();
		Node *ifTrue = Statement();

		if(oper == "else") {
			Next();
			Node *ifFalse = Statement();
			return new NodeSelectionStmt(expr, ifTrue, ifFalse);
		}
		return new NodeSelectionStmt(expr, ifTrue, NULL);
	}
	return NULL;
}

Node* Parser::IterationStmt() {
	if(oper == "for") {
		Next();
		if(oper != "(")
			throw ParserException(currentToken, "Iteration statement without '('");

		Next();
		Node *forInit = Expression();

		if(oper != ";")
			throw ParserException(currentToken, "Iteration statement without separator in condition");

		Next();
		Node *forCond = Expression();

		if(oper != ";")
			throw ParserException(currentToken, "Iteration statement without separator in condition");

		Next();
		Node *forIter = Expression();

		if(oper != ")")
			throw ParserException(currentToken, "Iteration statement without ')'");

		Next();
		Node *stmt = Statement();

		return new NodeIterationFor(forInit, forCond, forIter, stmt);
	}
	if(oper == "while") {
		Next();
		if(oper != "(")
			throw ParserException(currentToken, "Iteration statement without '('");

		Next();
		Node *expr = Expression();

		if(expr == NULL)
			throw ParserException(currentToken, "Iteration statement without expression");

		if(oper != ")")
			throw ParserException(currentToken, "Iteration statement without ')'");

		Next();
		Node *stmt = Statement();

		return new NodeIterationWhile(expr, stmt);
	}
	if(oper == "do") {
		Next();
		Node *stmt = Statement();
		
		if(oper == "while") {
			Next();
			if(oper != "(")
				throw ParserException(currentToken, "Iteration statement without '('");

			Next();
			Node *expr = Expression();

			if(expr == NULL)
				throw ParserException(currentToken, "Iteration statement without expression");

			if(oper != ")")
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
	if(oper == "goto") {
		Next();
		Node *ident = PrimaryExpr();

		if(ident == NULL)
			throw ParserException(currentToken, "Jump statement without label");

		if(oper != ";")
			throw ParserException(currentToken, "Jump statement without ';'");

		Next();
		return new NodeJumpStmt("goto", ident);
	}
	if(oper == "continue" || oper == "break") {
		string _oper = oper;
		Next();
		if(oper != ";")
			throw ParserException(currentToken, "Jump statement without ';'");

		Next();
		return new NodeJumpStmt(_oper, NULL);
	}
	if(oper == "return") {
		Next();
		Node *expr = Expression();

		if(oper != ";")
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
		if(oper != ";")
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
	if(str == NULL && symStack->GetTopTable()->TypeAt(oper)) {
		string _oper = oper;
		Next();
		return symStack->GetTopTable()->FindType(_oper);
	}
	return NULL;
}

Node* Parser::InitDeclaratorList(Symbol *type) {
	Node *first = InitDeclarator(type);

	if(first != NULL) {
		if(oper != ",")
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
		if(oper != "=")
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
	if(oper == "{") {
		Next();
		Node *init = InitialiserList();
		
		if(oper != "}")
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
		if(oper == ",") {
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
		string _oper = oper;
		Next();
		if(oper == "[") {
			Next();
			Node *link = ConditionalExpr();
			if(oper != "]")
				throw ParserException(currentToken, "Array declaration without ']'");

			Next();
		}
		symStack->GetTopTable()->AddVar(new SymVar(type, _oper));  //symtable add
		return new NodeVar(_oper);
	}
	return NULL;
}

Node* Parser::Pointer() {
	if(oper == "*") {
		Next();
		return Pointer();
	}
	return NULL;
}

//--- Struct Declaration ---

Node* Parser::StructSpec() {
	if(oper == "struct") {
		string ident = "";
		Next();

		if(currentToken->GetType() == IDENTIFIER) {
			ident = oper;
			Next();
		}
		if(oper == "{") {
			Next();
			Node *decl = StructDeclarationList();

			if(decl == NULL)
				throw ParserException(currentToken, "Struct without field list");

			if(oper != "}")
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
		
		if(oper == ";")
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
		if(oper == ",") {
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