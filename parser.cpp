#include "parser.h"

Parser::Parser(Scanner *_scanner) {
	scanner = _scanner;
	currentToken = NULL;
	lastToken = NULL;
	top = NULL;
	InitHashes();

	Parse();
}

void Parser::Next() {
	scanner->Next();
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
			return new NodeStruct(left, link);
		}
		else throw ParserException("No left bracket");
	}
	if(oper == "." || oper == "->") {
		Next();
		Node *right = PrimaryExpr();
		if(right == NULL)
			throw ParserException("No left field");

		return new NodeStruct(left, right);
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
		Node *funcName = Declarator();

		if(funcName != NULL) {
			if(oper != "(")
				throw ParserException("Function definition without left bracket");

			Next();
			Node* args = DeclarationList();

			if(oper != ")")
				throw ParserException("Function definition without right bracket");

			Next();
			Node *stmt = CompoundStmt();

			return new NodeFunc(type, funcName, args, stmt);
		}
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
		if(oper != ",")
			return decl;

		Next();
		Node *link = DeclarationList();
		return new NodeBinary(decl, ",", link);
	}
	return NULL;
}

Node* Parser::Declaration() {
	Symbol *decl = TypeSpec();

	if(decl != NULL) {
		return Declarator();
	}
	return NULL;
}

Symbol* Parser::TypeSpec() {
	if(typeName[oper]) { //таблица символов..
		string _oper = oper;
		Next();

		if(_oper == "int")
			return new SymTypeInteger();

		if(_oper == "float")
			return new SymTypeFloat();

		return new SymTypeScalar();
	}
	return NULL;
}

Node* Parser::InitDeclaratorList() {
	Node *decl = InitDeclarator();
	
	return NULL;
}

Node* Parser::InitDeclarator() {
	Node *decl = Declarator();
	
	if(oper == "=") {
		Next();
		Node *initialiser = AssignmentExpr();
		//making...
	}
	return decl;
}

Node* Parser::Declarator() {
	Node *decl = DirectDeclarator();

	return decl;
}

Node* Parser::DirectDeclarator() {
	if(currentToken->GetType() == IDENTIFIER) {
		string _oper = oper;
		Next();
		return new NodeVar(_oper);
	}
	return NULL;
}

Node* Parser::Pointer() {
	//from declarator
	return NULL;
}

//--- Init Hashes ---

void Parser::InitHashes() {
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
}