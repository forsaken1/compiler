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

string NodeConst::GetName() {
	return constant; 
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

void NodeVar::GenerateLeft(CodeGen *cg) {
	cg->AddCommand(LEA, EAX, name);
	cg->AddCommand(PUSH, EAX);
}

string NodeVar::GetName() { 
	return name; 
}

//--- NodeCall ---

NodeCall::NodeCall(SymbolType _type, string _name, Node *_args): NodeVar(_name) {
	type = _type;
	args = _args;
	name = "_" + name;
}

void NodeCall::Print(int i, bool b) { 
	cout << name << endl;
	if(args != NULL) {
		DrawPath(i, b);
		args->Print(i + 1, false);
	}
}

void NodeCall::Generate(CodeGen *cg) {
	if(args != NULL)
		args->Generate(cg);

	cg->AddCommand(CALL, name);
	
	if(type == TYPE_VOID)
		cg->AddCommand(PUSH, EAX);
}

//--- NodeLocalVar ---

void NodeLocalVar::Generate(CodeGen *cg) {
	cg->AddCommand(POP, name);
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

void NodeStruct::Generate(CodeGen *cg) {
	
}

//--- NodeUnary ---

NodeUnary::NodeUnary(TokenValue tv, Node *_right) {
	opval = tv;
	opname = GetTokenName(tv);
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
		case OPER_DEC:			Dec(cg); break;
		case OPER_INC:			Inc(cg); break;
		case OPER_MULTIPLY:		Dereference(cg); break;
		case OPER_PLUS:			Plus(cg); break;
		case OPER_MINUS:		Minus(cg); break;
		case OPER_BINARY_NOT:	BinaryNot(cg); break;
		case OPER_NOT:			Not(cg); break;
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
	cg->AddCommand(NOT, EAX);
}

void NodeUnary::Not(CodeGen *cg) {
	cg->AddCommand(NOT, EAX);
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

NodeBinary::NodeBinary(Node *_left, TokenValue tv, Node *_right) {
	left = _left;
	opval = tv;
	opname = GetTokenName(tv);
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
		case COMMA:						Comma(cg); return;
		case OPER_ASSIGN:				Assign(cg); return;
		case OPER_PLUS_ASSIGN:			PlusAssign(cg); return;
		case OPER_MINUS_ASSIGN:			MinusAssign(cg); return;
		case OPER_MULTIPLY_ASSIGN:		MultiplyAssign(cg); return;
		case OPER_DIVIDE_ASSIGN:		DivideAssign(cg); return;
		case OPER_DIVIDE_BY_MOD_ASSIGN:	DivideByModAssign(cg); return;	
		case OPER_EXCLUSIVE_OR_ASSIGN:	ExclusiveOrAssign(cg); return;
		case OPER_BINARY_AND_ASSIGN:	BinaryAndAssign(cg); return;
		case OPER_BINARY_OR_ASSIGN:		BinaryOrAssign(cg); return;
		case ARRAY_INDEX:				ArrayIndex(cg); return;
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
		case OPER_BINARY_AND:			BinaryAnd(cg); break;
		case OPER_BINARY_OR:			BinaryOr(cg); break;
		case OPER_XCHANGE:				XChange(cg); return;
		case OPER_ARROW:				break;
		case OPER_POINT:				break;
	}
	cg->AddCommand(PUSH, EAX);
}

void NodeBinary::ArrayIndex(CodeGen *cg) {
	//cg->AddCommand(MOV, EAX, left->GetName() + " + " + right->GetName());
	//cg->AddCommand(PUSH, EAX);
}

//------------------------------

void NodeBinary::XChange(CodeGen *cg) {
	cg->AddCommand(XCHG, EAX, EBX);
	cg->AddCommand(MOV, left->GetName(), EAX);
	cg->AddCommand(MOV, right->GetName(), EBX);
}

void NodeBinary::Comma(CodeGen *cg) {
	left->Generate(cg);
	cg->AddCommand(POP, EAX);
	right->Generate(cg);
	cg->AddCommand(POP, EAX);
}

void NodeBinary::Assign(CodeGen *cg) {
	left->GenerateLeft(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(MOV, _EAX, EBX);
	cg->AddCommand(PUSH, _EAX);
}

void NodeBinary::PlusAssign(CodeGen *cg) {
	left->GenerateLeft(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(ADD, _EAX, EBX);
	cg->AddCommand(PUSH, _EAX);
}

void NodeBinary::MinusAssign(CodeGen *cg) {
	left->GenerateLeft(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(SUB, _EAX, EBX);
	cg->AddCommand(PUSH, _EAX);
}

void NodeBinary::MultiplyAssign(CodeGen *cg) {
	left->GenerateLeft(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, ECX);
	cg->AddCommand(MOV, EAX, _ECX);
	cg->AddCommand(MUL, EBX);
	cg->AddCommand(MOV, _ECX, EAX);
	cg->AddCommand(PUSH, _ECX);
}

void NodeBinary::DivideAssign(CodeGen *cg) {
	left->GenerateLeft(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, ECX);
	cg->AddCommand(MOV, EAX, _ECX);
	cg->AddCommand(MOV, EDX, DEF);
	cg->AddCommand(DIV, EBX);
	cg->AddCommand(MOV, _ECX, EAX);
	cg->AddCommand(PUSH, _ECX);
}

void NodeBinary::DivideByModAssign(CodeGen *cg) {
	left->GenerateLeft(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, ECX);
	cg->AddCommand(MOV, EAX, _ECX);
	cg->AddCommand(MOV, EDX, DEF);
	cg->AddCommand(DIV, EBX);
	cg->AddCommand(MOV, _ECX, EDX);
	cg->AddCommand(PUSH, _ECX);
}

void NodeBinary::ExclusiveOrAssign(CodeGen *cg) {
	left->GenerateLeft(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(XOR, _EAX, EBX);
	cg->AddCommand(PUSH, _EAX);
}

void NodeBinary::BinaryAndAssign(CodeGen *cg) {
	left->GenerateLeft(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(AND, _EAX, EBX);
	cg->AddCommand(PUSH, _EAX);
}

void NodeBinary::BinaryOrAssign(CodeGen *cg) {
	left->GenerateLeft(cg);
	right->Generate(cg);
	cg->AddCommand(POP, EBX);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(OR, _EAX, EBX);
	cg->AddCommand(PUSH, _EAX);
}

//------------------------------

void NodeBinary::BinaryAnd(CodeGen *cg) {
	cg->AddCommand(AND, EAX, EBX);
}

void NodeBinary::BinaryOr(CodeGen *cg) {
	cg->AddCommand(OR, EAX, EBX);
}

void NodeBinary::ShiftLeft(CodeGen *cg) {
	cg->AddCommand(MOV, ECX, ONE);
	cg->AddCommand(RCL, EAX, CL);
}

void NodeBinary::ShiftRight(CodeGen *cg) {
	cg->AddCommand(MOV, ECX, ONE);
	cg->AddCommand(RCR, EAX, CL);
}

void NodeBinary::MoreEqual(CodeGen *cg) {
	cg->AddCommand(CMP, EAX, EBX);
	cg->AddCommand(MOV, EAX, DEF);

	string labelTrue =  GetRandomId("label_");
	string labelFalse = GetRandomId("label_");

	cg->JumpLabel(JGE, labelTrue);
	cg->JumpLabel(JMP, labelFalse);
	cg->AddLabel(labelTrue);
	cg->AddCommand(MOV, EAX, ONE);
	cg->AddLabel(labelFalse);
}

void NodeBinary::LessEqual(CodeGen *cg) {
	cg->AddCommand(CMP, EAX, EBX);
	cg->AddCommand(MOV, EAX, DEF);

	string labelTrue =  GetRandomId("label_");
	string labelFalse = GetRandomId("label_");

	cg->JumpLabel(JLE, labelTrue);
	cg->JumpLabel(JMP, labelFalse);
	cg->AddLabel(labelTrue);
	cg->AddCommand(MOV, EAX, ONE);
	cg->AddLabel(labelFalse);
}

void NodeBinary::More(CodeGen *cg) {
	cg->AddCommand(CMP, EAX, EBX);
	cg->AddCommand(MOV, EAX, DEF);

	string labelTrue =  GetRandomId("label_");
	string labelFalse = GetRandomId("label_");

	cg->JumpLabel(JG, labelTrue);
	cg->JumpLabel(JMP, labelFalse);
	cg->AddLabel(labelTrue);
	cg->AddCommand(MOV, EAX, ONE);
	cg->AddLabel(labelFalse);
}

void NodeBinary::Less(CodeGen *cg) {
	cg->AddCommand(CMP, EAX, EBX);
	cg->AddCommand(MOV, EAX, DEF);

	string labelTrue =  GetRandomId("label_");
	string labelFalse = GetRandomId("label_");

	cg->JumpLabel(JL, labelTrue);
	cg->JumpLabel(JMP, labelFalse);
	cg->AddLabel(labelTrue);
	cg->AddCommand(MOV, EAX, ONE);
	cg->AddLabel(labelFalse);
}

void NodeBinary::Equal(CodeGen *cg) {
	cg->AddCommand(CMP, EAX, EBX);
	cg->AddCommand(MOV, EAX, DEF);

	string labelTrue =  GetRandomId("label_");
	string labelFalse = GetRandomId("label_");

	cg->JumpLabel(JE, labelTrue);
	cg->JumpLabel(JMP, labelFalse);
	cg->AddLabel(labelTrue);
	cg->AddCommand(MOV, EAX, ONE);
	cg->AddLabel(labelFalse);
}

void NodeBinary::NotEqual(CodeGen *cg) {
	cg->AddCommand(CMP, EAX, EBX);
	cg->AddCommand(MOV, EAX, DEF);

	string labelTrue =  GetRandomId("label_");
	string labelFalse = GetRandomId("label_");

	cg->JumpLabel(JNE, labelTrue);
	cg->JumpLabel(JMP, labelFalse);
	cg->AddLabel(labelTrue);
	cg->AddCommand(MOV, EAX, ONE);
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
	cg->AddCommand(MOV, EDX, DEF);
	cg->AddCommand(DIV, EBX);
}

void NodeBinary::DivideByMod(CodeGen *cg) {
	cg->AddCommand(MOV, EDX, DEF);
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

NodeFunc::NodeFunc(SymTable *_table, Symbol *_returnValue, Node *_name, Node *_args, Node *_stmt) {
	returnValue = _returnValue;
	stmt = _stmt;
	name = _name;
	args = _args;
	table = _table;
}

void NodeFunc::Print(int i, bool b) {
	cout << returnValue->GetName() << " function ";
	name->Print(i + 1, true);

	if(args != NULL) {
		DrawPath(i, b);
		args->Print(i + 1, stmt != NULL);
	}
	if(stmt != NULL) {
		DrawPath(i, b);
		stmt->Print(i + 1, false);
	}
}

Symbol* NodeFunc::GetType() {
	return returnValue;
}

void NodeFunc::Generate(CodeGen *cg) {
	table->Print();

	cg->AddProc(name->GetName());

	if(args != NULL) {
		cg->AddCommand(POP, EBP);
		args->Generate(cg);
		cg->AddCommand(PUSH, EBP);
	}

	stmt->Generate(cg);
	cg->AddCommand(RET);

	cg->AddProcEnd(name->GetName());
}

//--- NodePause ---

void NodePause::Generate(CodeGen *cg) {
	cg->AddCommand(INKEY);
}

void NodePause::Print(int i, bool b) {
	cout << "pause" << endl;
}

//--- NodePrint ---

NodePrint::NodePrint(TokenValue _type, Node *_format, Node *_expr) {
	type = _type;
	format = _format;
	expr = _expr;

	switch(type) {
		case KEYWORD_PRINT: func = CRT_PRINTF; funcStrName = "print"; break;
		case KEYWORD_SCAN:  func = CRT_SCANF;  funcStrName = "scan"; break;
		default: break;
	}
}

void NodePrint::Print(int i, bool b) {
	cout << funcStrName << endl;
	DrawPath(i, b);
	format->Print(i + 1, true);

	if(expr != NULL) {
		DrawPath(i, b);
		expr->Print(i + 1, false);
	}
}

void NodePrint::Generate(CodeGen *cg) {
	if(expr == NULL) {
		cg->AddCommand(INVOKE, func, format->GetId());
	}
	else {
		if(func == CRT_PRINTF) {
			expr->Generate(cg);
			cg->AddCommand(POP, EAX);
			cg->AddCommand(INVOKE, func, format->GetId(), EAX);
		}
		else {
			cg->AddCommand(INVOKE, func, format->GetId(), expr->GetName());
		}
	}
}

//--- NodeSafeStmt ---

NodeSafeStmt::NodeSafeStmt(Node *_first) {
	first = _first;
	second = NULL;
}

NodeSafeStmt::NodeSafeStmt(Node *_first, Node *_second) {
	first = _first;
	second = _second;
}

void NodeSafeStmt::Print(int i, bool b) {
	if(second == NULL) {
		first->Print(i, false);
		return;
	}
	cout << "(,)" << endl;
	if(first != NULL) {
		DrawPath(i, b);
		first->Print(i + 1, second != NULL);
	}
	if(second != NULL) {
		DrawPath(i, b);
		second->Print(i + 1, false);
	}
}

void NodeSafeStmt::Generate(CodeGen *cg) {
	if(first != NULL) {
		first->Generate(cg);
		cg->AddCommand(POP, EAX);
	}
	if(second != NULL) {
		second->Generate(cg);
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
	if(first != NULL) {
		first->Generate(cg);
	}
	if(second != NULL) {
		second->Generate(cg);
	}
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

void NodeIterationWhile::Generate(CodeGen *cg) {
	string repeat = GetRandomId("label_while_repeat_");
	string out = GetRandomId("label_while_out_");

	cg->AddLabel(repeat);

	cond->Generate(cg);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(CMP, EAX, "0");
	cg->JumpLabel(JE, out);
	
	cg->GetStack()->Push(new Record(repeat, out));
	stmt->Generate(cg);

	cg->JumpLabel(JMP, repeat);
	cg->AddLabel(out);
	
	cg->GetStack()->Pop();
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

void NodeIterationDo::Generate(CodeGen *cg) {
	string repeat = GetRandomId("label_do_repeat_");
	string out = GetRandomId("label_do_out_");

	cg->AddLabel(repeat);
	
	cg->GetStack()->Push(new Record(repeat, out));
	stmt->Generate(cg);

	cond->Generate(cg);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(CMP, EAX, "0");
	cg->JumpLabel(JE, out);
	
	cg->JumpLabel(JMP, repeat);
	cg->AddLabel(out);
	
	cg->GetStack()->Pop();
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

void NodeIterationFor::Generate(CodeGen *cg) {
	string repeat = GetRandomId("label_for_repeat_");
	string out = GetRandomId("label_for_out_");

	if(init != NULL) {
		init->Generate(cg);
		cg->AddCommand(POP, EAX);
	}
	cg->AddLabel(repeat);

	if(cond != NULL) {
		cond->Generate(cg);
		cg->AddCommand(POP, EAX);
		cg->AddCommand(CMP, EAX, "0");
		cg->JumpLabel(JE, out);
	}
	
	cg->GetStack()->Push(new Record(repeat, out));
	stmt->Generate(cg);

	if(iter != NULL) {
		iter->Generate(cg);
		cg->AddCommand(POP, EAX);
	}
	cg->JumpLabel(JMP, repeat);
	cg->AddLabel(out);
	
	cg->GetStack()->Pop();
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
	trueStmt->Print(i + 1, falseStmt != NULL);

	if(falseStmt != NULL) {
		DrawPath(i, b);
		falseStmt->Print(i + 1, false);
	}
}

void NodeSelectionStmt::Generate(CodeGen *cg) {
	string labelTrue = GetRandomId("label_if_true_");
	string labelFalse = GetRandomId("label_if_false_");
	string labelOut = GetRandomId("label_if_out_");

	expr->Generate(cg);
	cg->AddCommand(POP, EAX);
	cg->AddCommand(CMP, EAX, "1");

	cg->JumpLabel(JE, labelTrue);

	if(falseStmt != NULL) {
		cg->JumpLabel(JNE, labelFalse);
		cg->AddLabel(labelTrue);

		trueStmt->Generate(cg);

		cg->JumpLabel(JMP, labelOut);
		cg->AddLabel(labelFalse);

		falseStmt->Generate(cg);
	}
	else {
		cg->JumpLabel(JMP, labelOut);
		cg->AddLabel(labelTrue);

		trueStmt->Generate(cg);
	}
	cg->AddLabel(labelOut);
}

//--- NodeJumpStmt ---

NodeJumpStmt::NodeJumpStmt(TokenValue _tv, Node *_expr) {
	tv = _tv;
	name = GetTokenName(tv);
	expr = _expr;
}

void NodeJumpStmt::Print(int i, bool b) {
	cout << "(" << name << ")" << endl;
	if(expr != NULL) {
		DrawPath(i, b);
		expr->Print(i + 1, b);
	}
}

void NodeJumpStmt::Generate(CodeGen *cg) {
	switch(tv) {
		case KEYWORD_RETURN:	Return(cg); break;
		case KEYWORD_CONTINUE:	Continue(cg); break;
		case KEYWORD_BREAK:		Break(cg); break;
		case KEYWORD_GOTO:		Goto(cg); break;
	}
}

void NodeJumpStmt::Return(CodeGen *cg) {
	if(expr != NULL) {
		cg->AddCommand(POP, EBP);
		expr->Generate(cg);
		cg->AddCommand(PUSH, EBP);
	}
	cg->AddCommand(RET);
}

void NodeJumpStmt::Continue(CodeGen *cg) {
	cg->AddCommand(JMP, cg->GetStack()->GetTop()->first);
}

void NodeJumpStmt::Break(CodeGen *cg) {
	cg->AddCommand(JMP, cg->GetStack()->GetTop()->second);
}

void NodeJumpStmt::Goto(CodeGen *cg) {
	cg->AddCommand(JMP, "label_" + expr->GetName());
}
