#include "node.h"

//--- GetRandomId ---

#include <stdlib.h>
#include <time.h>

string _GetRandomId(string str) {
	string random = str;

	for(int j = 0; j < 10; j++)
		random += to_string( (_Longlong)(rand() % 10) );

	return random;
}

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
		case OPER_DEC:			return "--";
		case OPER_INC:			return "++";
		case OPER_MULTIPLY:		return "*";
		case OPER_PLUS:			return "+";
		case OPER_MINUS:		return "-";
		case OPER_BINARY_NOT:	return "~";
		case OPER_NOT:			return "!";

		case FUNCTION_ARGUMENT: return "()";
		case SIZE_OF:			return "sizeof";
		case CAST_INT:			return "int";
		case CAST_CHAR:			return "char";
		case CAST_FLOAT:		return "float";
		default:				return "";
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
	cg->AddCommand(NOT, EAX);
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
		case OPER_BINARY_AND:			return "&";
		case OPER_AND:					return "&&";
		case OPER_BINARY_OR:			return "|";
		case OPER_OR:					return "||";
		case OPER_PLUS:					return "+";
		case OPER_MINUS:				return "-";
		case OPER_MULTIPLY:				return "*";
		case OPER_DIVIDE:				return "/";
		case OPER_DIVIDE_BY_MOD:		return "%";
		case OPER_ASSIGN:				return "=";
		case COMMA:						return ",";
		case OPER_EQUAL:				return "==";
		case OPER_NOT_EQUAL:			return "!=";
		case OPER_MORE:					return ">";
		case OPER_LESS:					return "<";
		case OPER_MORE_OR_EQUAL:		return ">=";
		case OPER_LESS_OR_EQUAL:		return "<=";
		case OPER_PLUS_ASSIGN:			return "+=";
		case OPER_MINUS_ASSIGN:			return "-=";
		case OPER_MULTIPLY_ASSIGN:		return "*=";
		case OPER_DIVIDE_ASSIGN:		return "/=";
		case OPER_DIVIDE_BY_MOD_ASSIGN:	return "%=";
		case OPER_BINARY_AND_ASSIGN:	return "&=";
		case OPER_BINARY_OR_ASSIGN:		return "|=";
		case OPER_SHIFT_LEFT:			return "<<";
		case OPER_SHIFT_RIGHT:			return ">>";
		case OPER_EXCLUSIVE_OR:			return "^";
		case OPER_EXCLUSIVE_OR_ASSIGN:	return "^=";
		case OPER_ARROW:				return "->";
		case OPER_POINT:				return ".";
		case ARRAY_INDEX:				return "[]";
		case FUNCTION_ARGUMENT:			return "()";
		default:						return "";
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
		case OPER_ASSIGN:				Assign(cg); return;
		case COMMA:						Comma(cg); return;
		case OPER_PLUS_ASSIGN:			PlusAssign(cg); return;
		case OPER_MINUS_ASSIGN:			MinusAssign(cg); return;
		case OPER_MULTIPLY_ASSIGN:		MultiplyAssign(cg); return;
		case OPER_DIVIDE_ASSIGN:		DivideAssign(cg); return;
		case OPER_DIVIDE_BY_MOD_ASSIGN:	DivideByModAssign(cg); return;	
		case OPER_EXCLUSIVE_OR_ASSIGN:	ExclusiveOrAssign(cg); return;
	}
	
	left->Generate(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, EAX);

	switch(opval) {
		case OPER_PLUS:					Add(cg); break;
		case OPER_MINUS:				Sub(cg); break;
		case OPER_MULTIPLY:				Mul(cg); break;
		case OPER_DIVIDE:				Div(cg); break;
		case OPER_DIVIDE_BY_MOD:		DivideByMod(cg); break;
		case OPER_AND:					And(cg); break;
		case OPER_OR:					Or(cg); break;
		case OPER_EXCLUSIVE_OR:			Xor(cg); break;
		case OPER_SHIFT_LEFT:			ShiftLeft(cg); break;
		case OPER_SHIFT_RIGHT:			ShiftRight(cg); break;
		case OPER_MORE:					More(cg); break;
		case OPER_LESS:					Less(cg); break;
		case OPER_EQUAL:				Equal(cg); break;
		case OPER_NOT_EQUAL:			NotEqual(cg); break;
		case OPER_MORE_OR_EQUAL:		MoreEqual(cg); break;
		case OPER_LESS_OR_EQUAL:		LessEqual(cg); break;
		case OPER_BINARY_AND:			break;
		case OPER_BINARY_OR:			break;
		case OPER_BINARY_AND_ASSIGN:	break;
		case OPER_BINARY_OR_ASSIGN:		break;
		case OPER_ARROW:				break;
		case OPER_POINT:				break;
		case ARRAY_INDEX:				break;
		case FUNCTION_ARGUMENT:			break;
	}
	cg->AddCommand(PUSH, EAX);
}

//------------------------------
void NodeBinary::Comma(CodeGen *cg) {
	left->Generate(cg);
	right->Generate(cg);
}

void NodeBinary::Assign(CodeGen *cg) {
	right->Generate(cg);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(MOV, left->GetName(), EAX);
	cg->AddCommand(PUSH, left->GetName());
}

void NodeBinary::PlusAssign(CodeGen *cg) {
	right->Generate(cg);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(ADD, left->GetName(), EAX);
	cg->AddCommand(PUSH, left->GetName());
}

void NodeBinary::MinusAssign(CodeGen *cg) {
	right->Generate(cg);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(SUB, left->GetName(), EAX);
	cg->AddCommand(PUSH, left->GetName());
}

void NodeBinary::MultiplyAssign(CodeGen *cg) {
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(MOV, EAX, left->GetName());
	cg->AddCommand(MUL, EBX);
	cg->AddCommand(MOV, left->GetName(), EAX);
	cg->AddCommand(PUSH, left->GetName());
}

void NodeBinary::DivideAssign(CodeGen *cg) {
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(MOV, EAX, left->GetName());
	cg->AddCommand(MOV, EDX, "0");
	cg->AddCommand(DIV, EBX);
	cg->AddCommand(MOV, left->GetName(), EAX);
	cg->AddCommand(PUSH, left->GetName());
}

void NodeBinary::DivideByModAssign(CodeGen *cg) {
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(MOV, EAX, left->GetName());
	cg->AddCommand(MOV, EDX, "0");
	cg->AddCommand(DIV, EBX);
	cg->AddCommand(MOV, left->GetName(), EDX);
	cg->AddCommand(PUSH, left->GetName());
}

void NodeBinary::ExclusiveOrAssign(CodeGen *cg) {
	right->Generate(cg);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(XOR, left->GetName(), EAX);
	cg->AddCommand(PUSH, left->GetName());
}
//------------------------------

void NodeBinary::ShiftLeft(CodeGen *cg) {
	cg->AddCommand(MOV, ECX, "1");
	cg->AddCommand(RCL, EAX, CL);
}

void NodeBinary::ShiftRight(CodeGen *cg) {
	cg->AddCommand(MOV, ECX, "1");
	cg->AddCommand(RCR, EAX, CL);
}

void NodeBinary::MoreEqual(CodeGen *cg) {
	cg->AddCommand(CMP, EAX, EBX);
	cg->AddCommand(MOV, EAX, "0");

	string labelTrue =  _GetRandomId("label_");
	string labelFalse = _GetRandomId("label_");

	cg->SetLabel(JGE, labelTrue);
	cg->SetLabel(JMP, labelFalse);
	cg->AddLabel(labelTrue);
	cg->AddCommand(MOV, EAX, "1");
	cg->AddLabel(labelFalse);
}

void NodeBinary::LessEqual(CodeGen *cg) {
	cg->AddCommand(CMP, EAX, EBX);
	cg->AddCommand(MOV, EAX, "0");

	string labelTrue =  _GetRandomId("label_");
	string labelFalse = _GetRandomId("label_");

	cg->SetLabel(JLE, labelTrue);
	cg->SetLabel(JMP, labelFalse);
	cg->AddLabel(labelTrue);
	cg->AddCommand(MOV, EAX, "1");
	cg->AddLabel(labelFalse);
}

void NodeBinary::More(CodeGen *cg) {
	cg->AddCommand(CMP, EAX, EBX);
	cg->AddCommand(MOV, EAX, "0");

	string labelTrue =  _GetRandomId("label_");
	string labelFalse = _GetRandomId("label_");

	cg->SetLabel(JG, labelTrue);
	cg->SetLabel(JMP, labelFalse);
	cg->AddLabel(labelTrue);
	cg->AddCommand(MOV, EAX, "1");
	cg->AddLabel(labelFalse);
}

void NodeBinary::Less(CodeGen *cg) {
	cg->AddCommand(CMP, EAX, EBX);
	cg->AddCommand(MOV, EAX, "0");

	string labelTrue =  _GetRandomId("label_");
	string labelFalse = _GetRandomId("label_");

	cg->SetLabel(JL, labelTrue);
	cg->SetLabel(JMP, labelFalse);
	cg->AddLabel(labelTrue);
	cg->AddCommand(MOV, EAX, "1");
	cg->AddLabel(labelFalse);
}

void NodeBinary::Equal(CodeGen *cg) {
	cg->AddCommand(CMP, EAX, EBX);
	cg->AddCommand(MOV, EAX, "0");

	string labelTrue =  _GetRandomId("label_");
	string labelFalse = _GetRandomId("label_");

	cg->SetLabel(JE, labelTrue);
	cg->SetLabel(JMP, labelFalse);
	cg->AddLabel(labelTrue);
	cg->AddCommand(MOV, EAX, "1");
	cg->AddLabel(labelFalse);
}

void NodeBinary::NotEqual(CodeGen *cg) {
	cg->AddCommand(CMP, EAX, EBX);
	cg->AddCommand(MOV, EAX, "0");

	string labelTrue =  _GetRandomId("label_");
	string labelFalse = _GetRandomId("label_");

	cg->SetLabel(JNE, labelTrue);
	cg->SetLabel(JMP, labelFalse);
	cg->AddLabel(labelTrue);
	cg->AddCommand(MOV, EAX, "1");
	cg->AddLabel(labelFalse);
}

void NodeBinary::Xor(CodeGen *cg) {
	cg->AddCommand(XOR, EAX, EBX);
}

void NodeBinary::Sub(CodeGen *cg) {
	cg->AddCommand(SUB, EAX, EBX);
}

void NodeBinary::Mul(CodeGen *cg) {
	cg->AddCommand(MUL, EBX);
}

void NodeBinary::Div(CodeGen *cg) {
	cg->AddCommand(MOV, EDX, "0");
	cg->AddCommand(DIV, EBX);
}

void NodeBinary::DivideByMod(CodeGen *cg) {
	cg->AddCommand(MOV, EDX, "0");
	cg->AddCommand(DIV, EBX);
	cg->AddCommand(MOV, EAX, EDX);
}
void NodeBinary::And(CodeGen *cg) {
	cg->AddCommand(AND, EAX, EBX);
}

void NodeBinary::Or(CodeGen *cg) {
	cg->AddCommand(OR, EAX, EBX);
}

void NodeBinary::Add(CodeGen *cg) {
	cg->AddCommand(ADD, EAX, EBX);
}

//--- NodeFunc ---

NodeFunc::NodeFunc(Symbol *_returnValue, Node *_name, Node *_args, Node *_stmt) {
	returnValue = _returnValue;
	stmt = _stmt;
	name = _name;
	args = _args;
}

void NodeFunc::Print(int i, bool b) {
	cout << returnValue->GetName() << " function ";
	name->Print(i + 1, true);

	if(args != NULL) {
		DrawPath(i, b);
		args->Print(i + 1, stmt == NULL ? false : true);
	}

	if(stmt != NULL) {
		DrawPath(i, b);
		stmt->Print(i + 1, false);
	}
}

Symbol* NodeFunc::GetType() {
	return returnValue;
}

//--- NodePrint ---

NodePrint::NodePrint(Node *_format, Node *_expr) {
	format = _format;
	expr = _expr;
}

void NodePrint::Print(int i, bool b) {
	cout << "print" << endl;
	DrawPath(i, b);
	format->Print(i + 1, true);
	if(expr != NULL) {
		DrawPath(i, b);
		expr->Print(i + 1, false);
	}
}

void NodePrint::Generate(CodeGen *cg) {
	if(expr == NULL) {
		cg->AddCommand(INVOKE, CRT_PRINTF, format->GetId());
	}
	else {
		expr->Generate(cg);
		cg->AddCommand(POP, EAX);
		cg->AddCommand(INVOKE, CRT_PRINTF, format->GetId(), EAX);
	}
}

//--- NodeStmt ---

NodeStmt::NodeStmt(string _name, Node *_first, Node *_second) {
	name = _name;
	first = _first;
	second = _second;
}

void NodeStmt::Print(int i, bool b) {
	cout << name << endl;
	if(first != NULL) {
		DrawPath(i, b);
		first->Print(i + 1, second != NULL);
	}
	if(second != NULL) {
		DrawPath(i, b);
		second->Print(i + 1, false);
	}
}

void NodeStmt::Generate(CodeGen *cg) {
	if(first != NULL)
		first->Generate(cg);

	if(second != NULL)
		second->Generate(cg);
}

//--- NodeIterationWhile ---

NodeIterationWhile::NodeIterationWhile(Node *_cond, Node *_stmt) {
	cond = _cond;
	stmt = _stmt;
}

void NodeIterationWhile::Print(int i, bool b) {
	cout << "while" << endl;

	DrawPath(i, b);
	cond->Print(i + 1, true);

	DrawPath(i, b);
	stmt->Print(i + 1, false);
}

//--- NodeIterationDo ---

NodeIterationDo::NodeIterationDo(Node *_cond, Node *_stmt): NodeIterationWhile(_cond, _stmt) {}

void NodeIterationDo::Print(int i, bool b) {
	cout << "do" << endl;
	DrawPath(i, b);
	cond->Print(i + 1, true);

	DrawPath(i, b);
	stmt->Print(i + 1, false);
}

//--- NodeIterationFor ---

NodeIterationFor::NodeIterationFor(Node *_init, Node *_cond, Node *_iter, Node *_stmt): NodeIterationWhile(_cond, _stmt) {
	iter = _iter;
	init = _init;
}

void NodeIterationFor::Print(int i, bool b) {
	cout << "for" << endl;

	if(init != NULL) {
		DrawPath(i, b);
		init->Print(i + 1, true);
	}

	if(cond != NULL) {
		DrawPath(i, b);
		cond->Print(i + 1, true);
	}

	if(iter != NULL) {
		DrawPath(i, b);
		iter->Print(i + 1, true);
	}

	DrawPath(i, b);
	stmt->Print(i + 1, false);
}

//--- NodeSelectionStmt ---

NodeSelectionStmt::NodeSelectionStmt(Node *_expr, Node *_trueStmt, Node *_falseStmt) {
	expr = _expr;
	trueStmt = _trueStmt;
	falseStmt = _falseStmt;
}

void NodeSelectionStmt::Print(int i, bool b) {
	cout << "if" << endl;
	DrawPath(i, b);
	expr->Print(i + 1, true);

	DrawPath(i, b);
	trueStmt->Print(i + 1, falseStmt == NULL ? false : true);

	if(falseStmt != NULL) {
		DrawPath(i, b);
		falseStmt->Print(i + 1, false);
	}
}

void NodeSelectionStmt::Generate(CodeGen *cg) {
	string labelTrue = _GetRandomId("label_if_");
	string labelFalse = _GetRandomId("label_if_");
	string labelOut = _GetRandomId("label_if_");

	expr->Generate(cg);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(CMP, EAX, "1");

	cg->SetLabel(JE, labelTrue);

	if(falseStmt != NULL) {
		cg->SetLabel(JNE, labelFalse);
		cg->AddLabel(labelTrue);

		trueStmt->Generate(cg);
		cg->SetLabel(JMP, labelOut);
		cg->AddLabel(labelFalse);

		falseStmt->Generate(cg);
	}
	else {
		cg->SetLabel(JMP, labelOut);
		cg->AddLabel(labelTrue);

		trueStmt->Generate(cg);
	}
	cg->AddLabel(labelOut);
}

//--- NodeJumpStmt ---

NodeJumpStmt::NodeJumpStmt(string _name, Node *_expr) {
	name = _name;
	expr = _expr;
}

void NodeJumpStmt::Print(int i, bool b) {
	cout << "(" << name << ")" << endl;
	if(expr != NULL) {
		DrawPath(i, b);
		expr->Print(i + 1, b);
	}
}
