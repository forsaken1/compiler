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
		top = Expression();
		top->Print();
	}
	catch(ParserException &e) {
		cout << e.GetMessage() << endl;
	}
}

Node* Parser::Expression() {
	Node *left = AssignmentExpr();

	if(currentToken->GetText() == ",") {
		Next();
		Node *right = Expression();
	}
	return left;
}

Node* Parser::AssignmentExpr() {
	Node *left = ConditionalExpr();

	if(oper == "++" || oper == "--" || oper == "sizeof") { //unary-expr
		Next();
		if(assignmentOperator[oper]) {
			Next();
			Node *right = AssignmentExpr();
			//return ;
		}
	}
	if(unaryOperator[oper]) { //cast-expr

	}
	return left;
}

Node* Parser::ConditionalExpr() { 
	Node *left = BinaryOperationExpr(0);

	return left;
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
		Next();
		Node *right = BinaryOperationExpr(priority);
		return new NodeBinary(left, oper, right);
	}
	return left;
}

Node* Parser::CastExpr() {
	Node *left = NULL;

	if(oper == "(") {
		Next();
		//считать TypeName
		if(oper == ")") {
			Next();
			return new NodeUnary("(TypeName)", CastExpr());//исправить с TypeName
		}
	}
	left = UnaryExpr();

	return left;
}

Node* Parser::UnaryExpr() {
	Node *left = PostfixExpr();

	if(left == NULL) {
		string _oper = oper;
		Next();
		if(_oper == "++" || _oper == "--" || _oper == "sizeof") {
			return new NodeUnary(_oper, UnaryExpr());
		}
		if(unaryOperator[_oper])
			return new NodeUnary(_oper, CastExpr());
	}
	return left;
}

Node* Parser::PostfixExpr() {
	Node *left = PrimaryExpr();

	if(left == NULL) {
		left = PostfixExpr();

		if(oper == "[") {

		}
		if(oper == "(") {

		}
		if(oper == ".") {

		}
		if(oper == "->") {

		}
		if(oper == "++" || oper == "--") {

		}
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
	}
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

	return link;
}

Node* Parser::DeclarationStmt() {

	return NULL;
}

Node* Parser::SelectionStmt() {
	if(oper == "if" || oper == "switch") {
		Next();
		if(oper != "(")
			throw ParserException("Selection statement without left bracket");
		
		Next();
		Node *expr = Expression();

		Next();
		if(oper != "(")
			throw ParserException("Selection statement without right bracket");
		
		Next();
		Node *ifTrue = Statement();

		if(oper == "else") {
			Next();
			Node *ifFalse = Statement();
			return new NodeSelectionStmt();
		}
		return new NodeSelectionStmt(); //Разные конструкторы
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

		Next();
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

	return NULL;
}

//--- Parse Definition ---

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
}