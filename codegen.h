#ifndef CODEGEN_H
#define CODEGEN_H

#include <map>
#include <vector>
#include <string>
#include <iostream>

enum Register {
	EAX,
	EBX,
	ECX,
	EDX,
	ESP
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
//--- Binary operations ---
	MOV,
	ADD,
	SUB,
//--- Ternary operations ---
	INVOKE,
//--- Crt-functions ---
	CRT_PRINTF
};

class AsmCmd {
protected:
	Cmd cmd;
	string cmdStr;

	string GetReg(Register _reg) {
		switch(_reg) {
			case EAX: return "eax";
			case EBX: return "ebx";
			case ECX: return "ecx";
			case EDX: return "edx";
			case ESP: return "esp";
		}
	}

	string GetCmd(Cmd _cmd) {
		switch(_cmd) {
			case INKEY: return "inkey";
			case EXIT: return "exit";
			case INC: return "inc";
			case DEC: return "dec";
			case CALL: return "call";
			case PUSH: return "push";
			case POP: return "pop";
			case RET: return "ret";
			case MUL: return "mul";
			case DIV: return "div";
			case MOV: return "mov";
			case ADD: return "add";
			case SUB: return "sub";
			case INVOKE: return "invoke";
			case CRT_PRINTF: return "crt_printf";
		}
	}

public:
	AsmCmd(Cmd _cmd) {
		cmd = _cmd;
		cmdStr = GetCmd(cmd);
	}

	virtual void Print() {
		cout << "\t" << cmdStr << endl;
	}

	Cmd GetCmd() {
		return cmd;
	}
};

class AsmUnary: public AsmCmd {
	bool regMode; 

protected:
	string regStr;
	Register reg;

public:
	AsmUnary(Cmd _cmd, Register _reg): AsmCmd(_cmd) {
		reg = _reg;
		regStr = GetReg(reg);
		regMode = true;
	}
	
	AsmUnary(Cmd _cmd, string str): AsmCmd(_cmd) {
		regStr = str;
		regMode = false;
	}

	void Print() {
		if(regMode)
			cout << "\t" << cmdStr << " \t" << regStr << endl;
		else
			cout << "\t" << cmdStr << " \tdword ptr " << regStr << "" << endl;
	}
};

class AsmBinary: public AsmUnary {
	int regMode;

protected:
	string regSecondStr;
	Register regSecond;

public:
	AsmBinary(Cmd _cmd, Register _reg, Register __reg): AsmUnary(_cmd, _reg) {
		regSecond = __reg;
		regSecondStr = GetReg(regSecond);
		regMode = 0;
	}

	AsmBinary(Cmd _cmd, Register _reg, string str): AsmUnary(_cmd, _reg) {
		regSecondStr = str;
		regMode = 1;
	}

	AsmBinary(Cmd _cmd, string str, Register _reg): AsmUnary(_cmd, str) {
		regSecond = _reg;
		regSecondStr = GetReg(regSecond);
		regMode = 2;
	}

	void Print() {
		cout << "\t" << cmdStr;
		switch(regMode) {
			case 0: cout << " \t" << regStr << ", " << regSecondStr << endl; break;
			case 1: cout << " \t" << regStr << ", dword ptr " << regSecondStr << "" << endl; break;
			case 2: cout << " \tdword ptr " << regStr << ", " << regSecondStr << endl; break;
		}
	}
};

class AsmTernary: public AsmCmd {
	bool onlyString;

protected:
	string funcStr, format, regStr;
	Register reg;
	Cmd func;

public:
	AsmTernary(Cmd _cmd, Cmd _func, string str, Register _reg): AsmCmd(_cmd) {
		func = _func;
		format = str;
		reg = _reg;
		funcStr = GetCmd(func);
		regStr = GetReg(reg);
		onlyString = false;
	}

	AsmTernary(Cmd _cmd, Cmd _func, string str): AsmCmd(_cmd) {
		func = _func;
		format = str;
		funcStr = GetCmd(func);
		onlyString = true;
	}

	void Print() {
		if(onlyString)
			cout << "\t" << cmdStr << "\t" << funcStr << ", addr " << format << endl;
		else
			cout << "\t" << cmdStr << "\t" << funcStr << ", addr " << format << ", " << regStr << endl;
	}
};

class CodeGen {
	vector<AsmCmd*> command;

public:
	CodeGen() {}

	void Print() {
		for(int i = 0; i < command.size(); i++)
			command[i]->Print();
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
};

#endif CODEGEN_H