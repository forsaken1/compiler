#ifndef CODEGEN_H
#define CODEGEN_H

#include <string>

enum Register {
	EAX,
	EBX,
	ECX,
	EDX,
	ESP
};

enum CmdNoOp {
	INKEY,
	EXIT,
};

enum CmdUnary {
	INT, 
	DEC,
	CALL,
	PUSH,
	POP,
	RET
};

enum CmdBinary {
	MOV,
	ADD,
	SUB,
	MUL,
	DIV
};

enum CmdTernary {
	INVOKE
};

class AsmCmd {

public: 
	AsmCmd() {}
};

class AsmCmdNoOperand: public AsmCmd {
	
public:
	AsmCmdNoOperand(CmdNoOp _cmd) {

	}
};

class AsmCmdUnary: public AsmCmd {

public:
	AsmCmdUnary(CmdUnary _cmd) {

	}
};

class AsmCmdBinary: public AsmCmd {
	
public:
	AsmCmdBinary(CmdBinary _cmd) {

	}
};

class AsmCmdTernary: public AsmCmd {

public:
	AsmCmdTernary(CmdTernary _cmd) {

	}
};

class CodeGen {

	string GetNoOp(CmdNoOp op) {
		switch(op) {
			case INKEY: return "inkey";
			case EXIT:	return "exit";
			default:	return "";
		}
	}

	string GetUnary(CmdUnary op) {
		switch(op) {
			case INT:  return "int";
			case DEC:  return "dec";
			case CALL: return "call";
			case PUSH: return "push";
			case POP:  return "pop";
			case RET:  return "ret";
			default:   return "";
		}
	}

	string GetRegister(Register reg) {
		switch(reg) {
			case EAX: return "eax";
			case EBX: return "ebx";
			case ECX: return "ecx";
			case EDX: return "edx";
			case ESP: return "esp";
			default:  return "";
		}
	}

	string GetBinary(CmdBinary op) {
		switch(op) {
			case MOV: return "mov";
			case ADD: return "add";
			case SUB: return "sub";
			case MUL: return "mul";
			case DIV: return "div";
			default:  return "";
		}
	}

public:
	CodeGen() {}

	void AddCommand(CmdNoOp op) {
		cout << "\t" << GetNoOp(op) << endl;
	}

	void AddCommand(CmdUnary op, Register reg) {
		cout << "\t" << GetUnary(op) << " " << GetRegister(reg) << endl;
	}

	void AddCommand(CmdUnary op, string str) {
		cout << "\t" << GetUnary(op) << " " << str << endl;
	}

	void AddCommand(CmdBinary op, Register left, Register right) {
		cout << "\t" << GetBinary(op) << " " << GetRegister(left) << ", " << GetRegister(right) << endl;
	}
};

#endif CODEGEN_H