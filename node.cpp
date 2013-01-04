#include "node.h"

//--- Node ---

void Node::DrawPath(int i, bool b) {
	string str = b ? "|   " : "    ";
		
	for(int j = 0; j < i - 1; j++) cout << "    ";
	for(int j = i <= 0 ? 0 : i - 1; j < i; j++) cout << str;
	cout << "|" << endl;
	for(int j = 0; j < i - 1; j++) cout << "    ";
	for(int j = i <= 0 ? 0 : i - 1; j < i; j++) cout << str;
	cout << "+---";
}

//--- NodeConst ---

NodeConst::NodeConst(string _id, string _constant) {
	id = _id;
	constant = _constant;
}

void NodeConst::Print(int i, bool b) {
	cout << "(" << constant << ")" << endl;
}

string NodeConst::GetId() {
	return id;
}

void NodeConst::Generate(CodeGen *cg) {
	cg->AddCommand(PUSH, constant);
}

//--- NodeVar ---

NodeVar::NodeVar(string _name) {
	name = _name;
}

void NodeVar::Print(int i, bool b) {
	cout << "(" << name << ")" << endl;
}

Symbol* NodeVar::GetType(SymTable *symTable) {
	return symTable->FindType(name);
}

void NodeVar::Generate(CodeGen *cg) {
	cg->AddCommand(PUSH, name);
}

string NodeVar::GetName() { 
	return name; 
}

//--- NodeCall ---

NodeCall::NodeCall(string _name, Node *_args): NodeVar(_name) {
	args = _args;
}

void NodeCall::Print(int i, bool b) { 
	cout << "(";
	args->Print(i + 1, true);
	cout << ")";
}

//--- NodeStruct ---

NodeStruct::NodeStruct(string _name, Node *_elem) {
	name = _name;
	elem = _elem;
}

string NodeStruct::GetName() {
	return name;
}

void NodeStruct::Print(int i, bool b) {
	cout << "struct (" << name << ")" << endl;
	
	if(elem != NULL) {
		DrawPath(i, b);
		elem->Print(i + 1, false);
	}
}

//--- NodeUnary ---

string NodeUnary::GetOpName(TokenValue tv) {
	switch(tv) {
		case OPER_DEC: return "--";
		case OPER_INC: return "++";
		case OPER_MULTIPLY: return "*";
		case OPER_PLUS: return "+";
		case OPER_MINUS: return "-";
		case OPER_BINARY_NOT: return "~";
		case OPER_NOT: return "!";

		case FUNCTION_ARGUMENT: return "()";
		case SIZE_OF: return "sizeof";
		case CAST_INT: return "int";
		case CAST_CHAR: return "char";
		case CAST_FLOAT: return "float";
	}
}

NodeUnary::NodeUnary(TokenValue tv, Node *_right) {
	opval = tv;
	opname = GetOpName(tv);
	right = _right;
}

void NodeUnary::Print(int i, bool b) {
	cout << "(" << opname << ")" << endl;

	DrawPath(i, b);
	right->Print(i + 1, false);
}

Symbol* NodeUnary::GetType(SymTable *symTable) {
	return right->GetType(symTable);
}

void NodeUnary::Generate(CodeGen *cg) {
	right->Generate(cg);
	cg->AddCommand(POP, EAX);

	switch(opval) {
		case OPER_DEC: Dec(cg); break;
		case OPER_INC: Inc(cg); break;
		case OPER_MULTIPLY: Dereference(cg); break;
		case OPER_PLUS: Plus(cg); break;
		case OPER_MINUS: Minus(cg); break;
		case OPER_BINARY_NOT: BinaryNot(cg); break;
		case OPER_NOT: Not(cg); break;
	}
	cg->AddCommand(PUSH, EAX);
}

void NodeUnary::Dereference(CodeGen *cg) {
	
}

void NodeUnary::Plus(CodeGen *cg) {

}

void NodeUnary::Minus(CodeGen *cg) {
	cg->AddCommand(MOV, EBX, EAX);
	cg->AddCommand(SUB, EAX, EBX);
	cg->AddCommand(SUB, EAX, EBX);
}

void NodeUnary::BinaryNot(CodeGen *cg) {

}

void NodeUnary::Not(CodeGen *cg) {

}

void NodeUnary::Dec(CodeGen *cg) {
	cg->AddCommand(DEC, EAX);
	cg->AddCommand(MOV, right->GetName(), EAX);
}

void NodeUnary::Inc(CodeGen *cg) {
	cg->AddCommand(INC, EAX);
	cg->AddCommand(MOV, right->GetName(), EAX);
}

//--- NodeBinary ---

string NodeBinary::GetOpName(TokenValue tv) {
	switch(tv) {
		case OPER_BINARY_AND: return "&";
		case OPER_AND: return "&&";
		case OPER_BINARY_OR: return "|";
		case OPER_OR: return "||";
		case OPER_PLUS: return "+";
		case OPER_MINUS: return "-";
		case OPER_MULTIPLY: return "*";
		case OPER_DIVIDE: return "/";
		case OPER_DIVIDE_BY_MOD: return "%";
		case OPER_ASSIGN: return "=";
		case COMMA: return ",";
		case OPER_EQUAL: return "==";
		case OPER_NOT_EQUAL: return "!=";
		case OPER_MORE: return ">";
		case OPER_LESS: return "<";
		case OPER_MORE_OR_EQUAL: return ">=";
		case OPER_LESS_OR_EQUAL: return "<=";
		case OPER_PLUS_EQUAL: return "+=";
		case OPER_MINUS_EQUAL: return "-=";
		case OPER_MULTIPLY_EQUAL: return "*=";
		case OPER_DIVIDE_EQUAL: return "/=";
		case OPER_DIVIDE_BY_MOD_EQUAL: return "%=";
		case OPER_BINARY_AND_EQUAL: return "&=";
		case OPER_BINARY_OR_EQUAL: return "|=";
		case OPER_SHIFT_LEFT: return "<<";
		case OPER_SHIFT_RIGHT: return ">>";
		case OPER_EXCLUSIVE_OR: return "^";
		case OPER_EXCLUSIVE_OR_EQUAL: return "^=";
		case OPER_ARROW: return "->";
		case OPER_POINT: return ".";
		case ARRAY_INDEX: return "[]";
		case FUNCTION_ARGUMENT: return "()";
	}
}

NodeBinary::NodeBinary(Node *_left, TokenValue tv, Node *_right) {
	left = _left;
	opval = tv;
	opname = GetOpName(tv);
	right = _right;
}

void NodeBinary::Print(int i, bool b) {
	cout << "(" << opname << ")" << endl;

	DrawPath(i, b);
	left->Print(i + 1, true);

	DrawPath(i, b);
	right->Print(i + 1, false);
}

Symbol* NodeBinary::GetType(SymTable *symTable) {
	if(left->GetType(symTable) == right->GetType(symTable))
		return left->GetType(symTable);

	return NULL;
}

void NodeBinary::Generate(CodeGen *cg) {
	switch(opval) {
		case OPER_ASSIGN: Assign(cg); break;
		case OPER_PLUS: Add(cg); break;
		case OPER_MINUS: Sub(cg); break;
		case OPER_MULTIPLY: Mul(cg); break;
		case OPER_DIVIDE: Div(cg); break;
		case OPER_DIVIDE_BY_MOD: DivideByMod(cg); break;
		case OPER_BINARY_AND: break;
		case OPER_AND: break;
		case OPER_BINARY_OR: break;
		case OPER_OR: break;
		case COMMA: break;
		case OPER_EQUAL: break;
		case OPER_NOT_EQUAL: break;
		case OPER_MORE: break;
		case OPER_LESS: break;
		case OPER_MORE_OR_EQUAL: break;
		case OPER_LESS_OR_EQUAL: break;
		case OPER_PLUS_EQUAL: break;
		case OPER_MINUS_EQUAL: break;
		case OPER_MULTIPLY_EQUAL: break;
		case OPER_DIVIDE_EQUAL: break;
		case OPER_DIVIDE_BY_MOD_EQUAL: break;
		case OPER_BINARY_AND_EQUAL: break;
		case OPER_BINARY_OR_EQUAL: break;
		case OPER_SHIFT_LEFT: break;
		case OPER_SHIFT_RIGHT: break;
		case OPER_EXCLUSIVE_OR: break;
		case OPER_EXCLUSIVE_OR_EQUAL: break;
		case OPER_ARROW: break;
		case OPER_POINT: break;
		case ARRAY_INDEX: break;
		case FUNCTION_ARGUMENT: break;
	}
}

void NodeBinary::Assign(CodeGen *cg) {
	right->Generate(cg);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(MOV, left->GetName(), EAX);
}

void NodeBinary::Sub(CodeGen *cg) {
	left->Generate(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(SUB, EAX, EBX);
	cg->AddCommand(PUSH, EAX);
}

void NodeBinary::Mul(CodeGen *cg) {
	left->Generate(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(MUL, EBX);
	cg->AddCommand(PUSH, EAX);
}

void NodeBinary::Div(CodeGen *cg) {
	left->Generate(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(MOV, EDX, "0");
	cg->AddCommand(DIV, EBX);
	cg->AddCommand(PUSH, EAX);
}

void NodeBinary::DivideByMod(CodeGen *cg) {
	left->Generate(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(MOV, EDX, "0");
	cg->AddCommand(DIV, EBX);
	cg->AddCommand(PUSH, EDX);
}

void NodeBinary::Add(CodeGen *cg) {
	left->Generate(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(ADD, EAX, EBX);
	cg->AddCommand(PUSH, EAX);
}