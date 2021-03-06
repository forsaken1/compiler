#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include <string>
#include <map>
#include <iostream>

using namespace std;

enum SymbolType {
	CONSTANT,
	VARIABLE,
	FUNCTION,
	STRUCT,
	ARRAY,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_VOID,
	TYPE_CHAR,
	LABEL
};

class Symbol {
protected:
	string name;
	SymbolType symType;

public:
	Symbol() {}

	Symbol(string _name) {
		name = _name;
	}
	
	virtual string GetName() {
		return name;
	}

	virtual SymbolType GetSymType() {
		return symType;
	}

	virtual int GetLength() { return 0; }
};

class SymConst: public Symbol {
	string constant;

public:
	SymConst(string _name, string _constant): Symbol(_name) {
		constant = _constant;
		symType = CONSTANT;
	}

	string GetConst() {
		return constant;
	}
};

//--- SymLabel ---

class SymLabel: public Symbol {
public:
	SymLabel(string _name): Symbol(_name) {
		symType = LABEL;
	}
};

//--- SymType ---

class SymTypeVoid: public Symbol {
public:
	SymTypeVoid(): Symbol("void") {
		symType = TYPE_VOID;
	}
};

class SymTypeChar: public Symbol {
public:
	SymTypeChar(): Symbol("char") {
		symType = TYPE_CHAR;
	}
};

class SymTypeFloat: public Symbol {
public:
	SymTypeFloat(): Symbol("float") {
		symType = TYPE_FLOAT;
	}
};

class SymTypeInteger: public Symbol {
public:
	SymTypeInteger(): Symbol("int") {
		symType = TYPE_INT;
	}
};

class SymTypeArray: public Symbol {
	Symbol *elemType;
	int length;

public:
	SymTypeArray(Symbol *_elemType, string _name, int _l): Symbol(_name) {
		elemType = _elemType;
		length = _l;
		symType = ARRAY;
	}

	int GetLength() {
		return length;
	}
};

class SymTypeStruct: public Symbol {
public:
	SymTypeStruct(string _name): Symbol(_name) {
		symType = STRUCT;
	}
};

class SymTypePointer: public Symbol {
	Symbol *refType;

public:

};

//--- SymVar ---

class SymVar: public Symbol {
	Symbol *type;

public:
	SymVar(Symbol *_type, string _name): Symbol(_name) {
		type = _type;
		symType = VARIABLE;
	}
};

//--- SymFunction ---

class SymFunction: public Symbol {
	Symbol *type;

public:
	SymFunction(Symbol *_type, string _name): Symbol(_name) {
		type = _type;
		symType = FUNCTION;
	}

	SymbolType GetReturnType() {
		return type->GetSymType();
	}
};

//--- SymTable ---

class SymTable {
	map<string, Symbol*> *var, *type;
	map<string, SymConst*> *c;

public:
	SymTable();

	void Print();

	map<string, Symbol*>* GetTableVar();
	map<string, Symbol*>* GetTableType();
	map<string, SymConst*>* GetTableConst();

	bool VarAt(string);
	bool ConstAt(string);
	bool TypeAt(string);

	Symbol* FindVar(string);
	Symbol* FindType(string);
	SymConst* FindConst(string);
	
	void AddVar(Symbol*);
	void AddConst(SymConst*);
	void AddType(Symbol*);
};

//--- SymTableStack ---

class SymTableStack {

	class SymNode {
		SymNode *next;
		SymTable *table;

	public:
		SymNode(SymNode *_next, SymTable *_table) {
			next = _next;
			table = _table;
		}

		SymTable* GetTable() { return table; }
		SymNode* GetNext()	 { return next; }
	};

	SymNode *top;

public:
	SymTableStack();

	void Push(SymTable*);
	void Print();
	
	bool VarAt(string);
	bool ConstAt(string);
	bool TypeAt(string);

	Symbol* FindVar(string);
	Symbol* FindType(string);
	SymConst* FindConst(string);
	
	void AddVar(Symbol*);
	void AddConst(SymConst*);
	void AddType(Symbol*);

	SymTable* Pop();
	SymTable* GetTopTable();
};

#endif SYM_TABLE_H
