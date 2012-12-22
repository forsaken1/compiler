#ifndef CODEGEN_H
#define CODEGEN_H

class AsmCmd {

};

class AsmCmdNoOperand: public AsmCmd {

};

class AsmCmdUnary: public AsmCmd {

};

class AsmCmdBinary: public AsmCmd {

};

class CodeGen {

public:
	CodeGen();

	void AddCommand();

};

#endif CODEGEN_H