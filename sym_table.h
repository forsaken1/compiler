#ifndef SYM_TABLE_H
#define SYM_TABLE_H

class Symbol {
public:
	string name;

	Symbol(string _name) {
		name = _name;
	}
};

//--- SymVar ---

class SymVar: public Symbol {
public:
	SymType *type;

	SymVar(SymType *_type, string _name) {
		type = _type;
		name = _name;
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

//--- SymType ---

class SymType: public Symbol {

};

class SymTypeScalar: public SymType {

};

class SymTypeFloat: public SymTypeScalar {

};

class SymTypeInteger: public SymTypeScalar {

};

class SymTypeArray: public SymType {
public:
	SymType *elemType;
	int length;
}

class SymTypeRecord: public SymType {

};

class SymTypePointer: public SymType {
public:
	SymType *refType;
};

//--- SymTable ---

class SymTable {
	

public:

};

class SymTableStack: public SymTable {

};

#endif SYM_TABLE_H
