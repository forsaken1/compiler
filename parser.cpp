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
		return new NodeBinary(left, _oper, Expression());
	}
	return left;
}

Node* Parser::AssignmentExpr() {
	Node *left = ConditionalExpr();

	if(assignmentOperator[oper]) {
		string _oper = oper;
		Next();
		return new NodeBinary(left, _oper, AssignmentExpr());
	}
	return left;
}

Node* Parser::ConditionalExpr() { 
	Node *condition = BinaryOperationExpr(0);

	if(oper == "?") {
		Next();
		Node *cond_true = Expression();

		if(oper == ":") {
			Next();
			return new NodeTernary(condition, cond_true, ConditionalExpr());
		}
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
		return new NodeBinary(left, _oper, BinaryOperationExpr(priority));
	}
	return left;
}

Node* Parser::CastExpr() {
	if(typeName[oper] && lastToken->GetText() == "(") {
		string _oper = oper;
		Next();
		if(oper == ")") {
			Next();
			return new NodeUnary(_oper, CastExpr());
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
		return new NodeUnary(_oper, UnaryExpr());
	}
	if(unaryOperator[oper]) {
		string _oper = oper;
		Next();
		return new NodeUnary(_oper, CastExpr());
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
	}
	if(oper == "." || oper == "->") {
		Next();
		return new NodeStruct(left, PrimaryExpr());
	}
	if(oper == "++" || oper == "--") {
		string _oper = oper;
		Next();
		return new NodeUnaryPostfix(_oper, left);
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

	return NULL;
}

Node* Parser::StatementList() {

	return NULL;
}

Node* Parser::CompoundStmt() {
	Node *link = NULL;

	if(oper == "{") {
		Next();
		link = StatementList();
		
		if(link == NULL) link = DeclarationStmt();
		//Добавить StatementList();
	}
	return link;
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
	Symbol *decl = Declaration();
	//decl поместить в SymTable, если decl != null
	return NULL;
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

		return new NodeIterationStmt(); //Доделать конструктор
	}
	if(oper == "while") {
		Node();
		if(oper != "(")
			throw ParserException("Iteration statement without left bracket");

		Next();
		Node *expr = Expression();

		if(oper != ")")
			throw ParserException("Iteration statement without right bracket");

		Next();
		Node *stmt = Statement();

		return new NodeIterationStmt();
	}
	if(oper == "do") {
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
			return new NodeIterationStmt();
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
		return new NodeJumpStmt();
	}
	if(oper == "continue" || oper == "break") {
		Next();
		if(oper != ";")
			throw ParserException("Jump statement without ';'");

		Next();
		return new NodeJumpStmt();
	}
	if(oper == "return") {
		Node *expr = Expression();

		Next();
		if(oper != ";")
			throw ParserException("Jump statement without ';'");

		Next();
		return new NodeJumpStmt();
	}
	return NULL;
}

//--- Parse Declaration ---

Symbol* Parser::Declaration() {
	Symbol *decl = DeclarationSpec();

	return decl;
}

Symbol* Parser::DeclarationSpec() {
	Symbol *decl = TypeSpec();

	if(oper != ";") {
		Symbol *var = InitDeclarationList();
	}
	return decl;
}

Symbol* Parser::TypeSpec() {
	if(typeName[oper]) {
		Next();
		string _oper = oper;

		if(_oper == "int")
			return new SymTypeInteger();

		if(_oper == "float")
			return new SymTypeFloat();

		return new SymTypeScalar();
	}
	return NULL;
}

Symbol* Parser::InitDeclarationList() {
	Symbol *decl = InitDeclarator();
	
	return decl;
}

Symbol* Parser::InitDeclarator() {
	Symbol *decl = Declarator();
	
	if(oper == "=") {
		Next();
		Node *initialiser = AssignmentExpr();
		//making...
	}
	return decl;
}

Symbol* Parser::Declarator() {
	Symbol *decl = DirectDeclarator();

	if(currentToken->GetType() == IDENTIFIER) { //only scalar var.
		string _oper = oper;
		Next();
		return new Symbol(_oper);
	}
	return decl;
}

Symbol* Parser::DirectDeclarator() {
	
	return NULL;
}

Symbol* Parser::Pointer() {
	
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