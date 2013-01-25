#ifndef CODEGEN_H
#define CODEGEN_H

#include <map>
#include <vector>
#include <string>
#include <iostream>

enum Register {
	DEF,
	EAX,
	EBX,
	ECX,
	EDX,

	ESP,
	EBP,

	CL
};

enum Cmd {
//--- No operands ---
	INKEY,
	EXIT,
//--- Unary operations ---
	INC, 
	DEC,
	CALL,
	PUSH,
	POP,
	RET,
	MUL,
	DIV,
	NOT,
	JE,
	JL,
	JNL,
	JLE,
	JNLE,
	JNGE,
	JGE,
	JNG,
	JG,
	JMP,
	JNE,
//--- Binary operations ---
	MOV,
	ADD,
	SUB,
	CMP,
	XCHG,
	XOR,
	AND,
	OR,
	RCL,
	RCR,
//--- Ternary operations ---
	INVOKE,
//--- Crt-functions ---
	CRT_PRINTF,
	CRT_SCANF
};

class AsmCmd {
protected:
	Cmd cmd;
	string cmdStr;
	bool label;

	string GetReg(Register _reg) {
		switch(_reg) {
			case EAX:	return "eax";
			case EBX:	return "ebx";
			case ECX:	return "ecx";
			case EDX:	return "edx";

			case ESP:	return "esp";
			case EBP:	return "ebp";

			case CL:	return "cl";
			default:	return "";
		}
	}

	string GetCmd(Cmd _cmd) {
		switch(_cmd) {
			case JNE:	return "jne";
			case JMP:	return "jmp";
			case JE:	return "je";
			case JL:	return "jl";
			case JNL:	return "jnl";
			case JLE:	return "jle";
			case JNLE:	return "jnle";
			case JNGE:	return "jnge";
			case JGE:	return "jge";
			case JNG:	return "jng";
			case JG:	return "jg";
			case RCL:	return "rcl";
			case RCR:	return "rcr";
			case NOT:	return "not";
			case CMP:	return "cmp";
			case XCHG:	return "xchg";
			case XOR:	return "xor";
			case AND:	return "and";
			case OR:	return "or";
			case INKEY: return "inkey";
			case EXIT:	return "exit";
			case INC:	return "inc";
			case DEC:	return "dec";
			case CALL:	return "call";
			case PUSH:	return "push";
			case POP:	return "pop";
			case RET:	return "ret";
			case MUL:	return "mul";
			case DIV:	return "div";
			case MOV:	return "mov";
			case ADD:	return "add";
			case SUB:	return "sub";
			case INVOKE: return "invoke";
			case CRT_PRINTF: return "crt_printf";
			case CRT_SCANF: return "crt_scanf";
			default:	return "";
		}
	}

public:
	AsmCmd() {}

	AsmCmd(Cmd _cmd) {
		cmd = _cmd;
		cmdStr = GetCmd(cmd);
		label = false;
	}

	AsmCmd(string label) {
		cmdStr = label;
		label = true;
	}

	virtual void Print() {
		if(label)
			cout << endl << cmdStr << ":" << endl;
		else
			cout << "\t" << cmdStr << endl;
	}

	Cmd GetCmd() {
		return cmd;
	}

	virtual string GetFirst() {
		return "";
	}
};

//--- AsmProc ---

class AsmProc: public AsmCmd {
	string name;

public:
	AsmProc(string _name): AsmCmd() {
		name = _name;
	}

	void Print() {
		cout << endl << name << " proc" << endl;
	}
};

//--- AsmProcEnd

class AsmProcEnd: public AsmCmd {
	string name;

public:
	AsmProcEnd(string _name): AsmCmd() {
		name = _name;
	}

	void Print() {
		cout << endl << name << " endp" << endl << endl;
	}
};

//--- AsmLabel ---

class AsmLabel: public AsmCmd {
	string label;
	
public:
	AsmLabel(Cmd _op, string _label): AsmCmd(_op) {
		label = _label;
	}

	void Print() {
		cout << "\t" << cmdStr << "\t\t" << label << endl;
	}
};

//--- AsmUnary ---

class AsmUnary: public AsmCmd {
protected:
	string regStr;
	Register reg;

public:
	AsmUnary(Cmd _cmd, Register _reg): AsmCmd(_cmd) {
		reg = _reg;
		regStr = GetReg(reg);
	}
	
	AsmUnary(Cmd _cmd, string str): AsmCmd(_cmd) {
		regStr = str;
	}

	void Print() {
		cout << "\t" << cmdStr << " \t" << regStr << endl;
	}
	
	string GetFirst() {
		return regStr;
	}
};

//--- AsmBinary ---

class AsmBinary: public AsmUnary {
protected:
	string regSecondStr;
	Register regSecond;

public:
	AsmBinary(Cmd _cmd, Register _reg, Register __reg): AsmUnary(_cmd, _reg) {
		regSecond = __reg;
		regSecondStr = GetReg(regSecond);
	}

	AsmBinary(Cmd _cmd, Register _reg, string str): AsmUnary(_cmd, _reg) {
		regSecondStr = str;
	}

	AsmBinary(Cmd _cmd, string str, Register _reg): AsmUnary(_cmd, str) {
		regSecond = _reg;
		regSecondStr = GetReg(regSecond);
	}

	AsmBinary(Cmd _cmd, string str, string _str): AsmUnary(_cmd, str) {
		regSecondStr = _str;
	}

	void Print() {
		cout << "\t" << cmdStr << " \t" << regStr << ", " << regSecondStr << endl;
	}
};

//--- AsmTernary ---

class AsmTernary: public AsmCmd {
	bool first, second;

protected:
	string funcStr, format, regStr;
	Register reg;
	Cmd func;

public:
	AsmTernary(Cmd _cmd, Cmd _func, string str, string _reg): AsmCmd(_cmd) {
		func = _func;
		format = str;
		regStr = _reg;
		funcStr = GetCmd(func);
		first = false;
		second = true;
	}

	AsmTernary(Cmd _cmd, Cmd _func, string str, Register _reg): AsmCmd(_cmd) {
		func = _func;
		format = str;
		reg = _reg;
		funcStr = GetCmd(func);
		regStr = GetReg(reg);
		first = false;
		second = false;
	}

	AsmTernary(Cmd _cmd, Cmd _func, string str): AsmCmd(_cmd) {
		func = _func;
		format = str;
		funcStr = GetCmd(func);
		first = true;
		second = false;
	}

	void Print() {
		if(first)
			cout << "\t" << cmdStr << "\t" << funcStr << ", addr " << format << endl;
		else
			cout << "\t" << cmdStr << "\t" << funcStr << ", addr " << format << (second ? ", addr " : ", ") << regStr << endl;
	}
};

class Record {
	Record *next;

public:
	string first, second;

	Record(string f, string s) {
		first = f;
		second = s;
	}

	void SetNext(Record *n) {
		next = n;
	}

	Record* GetNext() {
		return next;
	}
};

//--- CodeGen ---

class CodeGen {
	class Stack {
		Record *top;
		
	public:
		Stack() {
			top = NULL;	 
		}

		void Push(Record *r) {
			Record *t = top;
			top = r;
			top->SetNext(t);
		}

		Record* Pop() {
			Record *t = top;
			top = top->GetNext();
			return t;
		}

		Record* GetTop() {
			return top;
		}
	};

	Stack *stack;
	vector<AsmCmd*> command;
	bool optimisation;

public:
	CodeGen(bool _optimisation) {
		optimisation = _optimisation;
		stack = new Stack();
	}

	Stack* GetStack() {
		return stack;
	}

	void Optimisation() {
		for(int i = 0; i < (int)command.size(); i++) {
			if(command[i]->GetCmd() == PUSH) {
				if(command[i + 1]->GetCmd() == POP) {
					if(command[i]->GetFirst() != command[i + 1]->GetFirst()) {
						string op = command[i]->GetFirst();
						delete command[i];
						command[i] = new AsmBinary(MOV, command[i + 1]->GetFirst(), op);
						command.erase(command.begin() + i + 1);
						continue;
					}
					if(command[i]->GetFirst() == command[i + 1]->GetFirst()) {
						command.erase(command.begin() + i);
						command.erase(command.begin() + i + 1);
						i--;
					}
				}
			}

			if(command[i]->GetCmd() == POP) {

			}
		}
	}

	void Print() {
		if(optimisation)
			Optimisation();

		for(int i = 0; i < (int)command.size(); i++)
			command[i]->Print();
	}

	void AddLabel(string label) {
		command.push_back(new AsmCmd(label));
	}

	void JumpLabel(Cmd op, string label) {
		command.push_back(new AsmLabel(op, label));
	}

	void AddProc(string name) {
		command.push_back(new AsmProc(name));
	}

	void AddProcEnd(string name) {
		command.push_back(new AsmProcEnd(name));
	}

	void AddCommand(Cmd op) {
		command.push_back(new AsmCmd(op));
	}

	void AddCommand(Cmd op, Register reg) {
		command.push_back(new AsmUnary(op, reg));
	}

	void AddCommand(Cmd op, string str) {
		command.push_back(new AsmUnary(op, str));
	}

	void AddCommand(Cmd op, Register left, Register right) {
		command.push_back(new AsmBinary(op, left, right));
	}

	void AddCommand(Cmd op, string str, Register reg) {
		command.push_back(new AsmBinary(op, str, reg));
	}

	void AddCommand(Cmd op, Register reg, string str) {
		command.push_back(new AsmBinary(op, reg, str));
	}
	
	void AddCommand(Cmd op, Cmd func, string str) {
		command.push_back(new AsmTernary(op, func, str));
	}

	void AddCommand(Cmd op, Cmd func, string str, Register right) {
		command.push_back(new AsmTernary(op, func, str, right));
	}

	void AddCommand(Cmd op, Cmd func, string str, string right) {
		command.push_back(new AsmTernary(op, func, str, right));
	}
};

#endif CODEGEN_H