#include "parser.h"

Parser::Parser(Scanner *_scanner) {
	scanner = _scanner;
	currentToken = NULL;
	lastToken = NULL;
	top = NULL;
	globalVar = new SymTable();
	globalType = new SymTable();
	InitTables();

	Parse();
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
		top = Statement();
		top->Print(0, true);
	}
	catch(ParserException &e) {
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
			throw ParserException("Enumeration without expression after ','");

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
			throw ParserException("Assignment expression without left operand");

		if(left->GetType(globalVar) != right->GetType(globalVar))
			throw ParserException("Operand types do not match");

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
			throw ParserException("Ternary expression without true-condition");

		if(oper == ":") {
			Next();
			Node *cond_false = ConditionalExpr();
			if(cond_false == NULL)
				throw ParserException("Ternary expression without false-condition");

			return new NodeSelectionStmt(condition, cond_true, cond_false);
		}
		else
			throw ParserException("Ternary expression without false-condition");
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
			throw ParserException("Binary expression without left operand");

		return new NodeBinary(left, _oper, right);
	}
	return left;
}

Node* Parser::CastExpr() {
	if(typeName[oper] && lastToken->GetText() == "(") {
		string _oper = oper;
		Next();
		if(oper == ")") {
			Next();
			Node *right = CastExpr();
			if(right == NULL)
				throw ParserException("Cast expression without left operand");

			return new NodeUnary(_oper, right);
		}
	}
	return UnaryExpr();
}

Node* Parser::UnaryExpr() {
	if(oper == "sizeof") {
		Next();
		if(oper == "(") {
			Next();
			string type = oper;
			if(typeName[oper]) {
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
			throw ParserException("No left operand");

		return new NodeUnary(_oper, right);
	}
	if(unaryOperator[oper]) {
		string _oper = oper;
		Next();
		Node *right = CastExpr();
		if(right == NULL)
			throw ParserException("No left operand");

		return new NodeUnary(_oper, right);
	}	
	return PostfixExpr();
}

Node* Parser::PostfixExpr() {
	Node *left = PrimaryExpr();

	if(oper == "[") {
		Next();
		Node *link = Expression();

		if(oper == "]") {
			Next();
			return new NodeBinary(left, "[]",  link);
		}
		else throw ParserException("No left bracket");
	}
	if(oper == "." || oper == "->") {
		string _oper = oper;
		Next();
		Node *right = PrimaryExpr();
		if(right == NULL)
			throw ParserException("No left field");

		return new NodeBinary(left, _oper, right);
	}
	if(oper == "++" || oper == "--") {
		string _oper = oper;
		Next();
		return new NodeUnary(_oper, left);
	}
	return left;
}

Node* Parser::PrimaryExpr() {
	if(currentToken->GetType() == IDENTIFIER) {
		Next();
		return new NodeVar(lastToken->GetText());
	}

	if(currentToken->GetType() == CONST_INTEGER ||
	   currentToken->GetType() == CONST_REAL ||
	   currentToken->GetType() == CONST_CHAR ||
	   currentToken->GetType() == CONST_STRING) {
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

Node* Parser::Statement() {
	Node *link = FunctionDefinitionStmt();

	if(link == NULL) link = CompoundStmt();
	if(link == NULL) link = ExpressionStmt();
	if(link == NULL) link = DeclarationStmt();
	if(link == NULL) link = SelectionStmt();
	if(link == NULL) link = IterationStmt();
	if(link == NULL) link = JumpStmt();

	return link;
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
				throw ParserException("Function definition without right bracket");

			Next();
			Node *stmt = CompoundStmt();

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
			throw ParserException("Function argument list without arguments");

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
		Next();
		Node *decl = DeclarationList();
		Node *stmt = StatementList();

		if(oper != "}")
			throw ParserException("Compound statement without close bracket");
		else
			Next();

		if(decl == NULL && stmt == NULL)
			return NULL;
		
		return new NodeStmt("{stmt}", decl, stmt);
	}
	return NULL;
}

Node* Parser::ExpressionStmt() {
	Node *link = Expression();

	if(oper == ";")
		Next();
	else 
		if(link != NULL)
			throw ParserException("Expression without ';'");

	return link;
}

Node* Parser::DeclarationStmt() {
	Node *decl = DeclarationList();

	return decl;
}

Node* Parser::SelectionStmt() {
	if(oper == "if" || oper == "switch") {
		Next();
		if(oper != "(")
			throw ParserException("Selection statement without left bracket");
		
		Next();
		Node *expr = Expression();

		if(oper != ")")
			throw ParserException("Selection statement without right bracket");
		
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
			throw ParserException("Iteration statement without left bracket");

		Next();
		Node *forInit = Expression();

		if(oper != ";")
			throw ParserException("Iteration statement without separator in condition");

		Next();
		Node *forCond = Expression();

		if(oper != ";")
			throw ParserException("Iteration statement without separator in condition");

		Next();
		Node *forIter = Expression();

		if(oper != ")")
			throw ParserException("Iteration statement without right bracket");

		Next();
		Node *stmt = Statement();

		return new NodeIterationFor(forInit, forCond, forIter, stmt);
	}
	if(oper == "while") {
		Next();
		if(oper != "(")
			throw ParserException("Iteration statement without left bracket");

		Next();
		Node *expr = Expression();

		if(oper != ")")
			throw ParserException("Iteration statement without right bracket");

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
				throw ParserException("Iteration statement without right bracket");

			Next();
			Node *expr = Expression();

			if(oper != ")")
				throw ParserException("Iteration statement without left bracket");

			Next();
			return new NodeIterationDo(expr, stmt);
		}
		else 
			throw ParserException("Iteration statement without condition");
	}
	return NULL;
}

Node* Parser::JumpStmt() {
	if(oper == "goto") {
		Next();
		Node *ident = PrimaryExpr();

		if(oper != ";")
			throw ParserException("Jump statement without ';'");

		Next();
		return new NodeJumpStmt("goto", ident);
	}
	if(oper == "continue" || oper == "break") {
		string _oper = oper;
		Next();
		if(oper != ";")
			throw ParserException("Jump statement without ';'");

		Next();
		return new NodeJumpStmt(_oper, NULL);
	}
	if(oper == "return") {
		Next();
		Node *expr = Expression();

		if(oper != ";")
			throw ParserException("Jump statement without ';'");

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
			throw ParserException("Init declarator list without ';'");
		else
			Next();

		return init;
	}
	return NULL;
}

Symbol* Parser::TypeSpec() {
	if(globalType->At(oper)) {
		string _oper = oper;
		Next();
		return globalType->Find(_oper);
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
			throw ParserException("Init declarator without initialiser");

		return new NodeBinary(decl, "=", init);
	}
	return NULL;
}

Node* Parser::Initialiser() {
	if(oper == "{") {
		Next();
		Node *init = InitialiserList();
		
		if(oper != "}")
			throw ParserException("Initialiser list without '}'");
		
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
				throw ParserException("Array declaration without ']'");

			Next();
		}
		globalVar->Add(_oper, type);//symtable add
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
		if(currentToken->GetType() == IDENTIFIER) {
			string ident = oper;
			Next();
		}
		if(oper == "{") {
			Node *decl = StructDeclarationList();
			///return
		}
	}
	return NULL;
}

Node* Parser::StructDeclarationList() {
	Node *first = StructDeclaration();

	if(first != NULL) {
		Node *second = StructDeclarationList();

		if(second == NULL)
			;//
		else
			;//
	}
	return NULL;
}

Node* Parser::StructDeclaration() {
	
	return NULL;
}

Node* Parser::SpecList() {
	
	return NULL;
}

Node* Parser::StructDeclaratorList() {
	
	return NULL;
}

Node* Parser::StructDeclarator() {
	
	return NULL;
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

	typeName["void"] = true;
	typeName["char"] = true;
	typeName["int"] = true;
	typeName["float"] = true;

	globalType->Add("void", new SymType());
	globalType->Add("char", new SymTypeScalar());
	globalType->Add("int",  new SymTypeInteger());
	globalType->Add("float", new SymTypeFloat());
}