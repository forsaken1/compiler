#ifndef CODEGEN_H
#define CODEGEN_H

#include <string>

enum Register {
	EAX,
	EBX,
	ECX,
	EDX
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

public:
	CodeGen() {}

	void AddCommand(AsmCmd) {}
};

#endif CODEGEN_H