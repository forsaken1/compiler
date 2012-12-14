#ifndef SYM_TABLE_H
#define SYM_TABLE_H

class Symbol {
protected:
	string name;

public:
	Symbol() {}

	Symbol(string _name) {
		name = _name;
	}
	
	virtual string GetName() {
		return name;
	}
};

//--- SymType ---

class SymType: public Symbol {
public:
	SymType() {}
};

class SymTypeScalar: public SymType {
public:
	SymTypeScalar() {}
};

class SymTypeFloat: public SymTypeScalar {
public:
	SymTypeFloat() {
		name = "float";
	}
};

class SymTypeInteger: public SymTypeScalar {
public:
	SymTypeInteger() {
		name = "int";
	}
};

class SymTypeArray: public SymType {
	SymType *elemType;
	int length;

public:
	
};

class SymTypeRecord: public SymType {

};

class SymTypePointer: public SymType {
	SymType *refType;

public:

};

//--- SymVar ---

class SymVar: public Symbol {
	SymType *type;

public:
	SymVar(SymType *_type, string _name): Symbol(_name) {
		type = _type;
	}
};

class SymVarParam: public SymVar {

};

class SymVarConst: public SymVar {

};

class SymVarLocal: public SymVar {

};

class SymVarGlobal: public SymVar {

};

//--- SymFunction ---

class SymFunc: public Symbol {
public:

};

//--- SymTable ---

class SymTable {
	

public:
	Symbol* Find();
};

class SymTableStack: public SymTable {

};

#endif SYM_TABLE_H
