#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include <string>
#include <map>
#include <iostream>

using namespace std;

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

class SymConst: public Symbol {
	string constant;

public:
	SymConst(string _name, string _constant): Symbol(_name) {
		constant = _constant;
	}

	string GetConst() {
		return constant;
	}
};

//--- SymType ---

class SymTypeVoid: public Symbol {
public:
	SymTypeVoid() {
		name = "void";
	}
};

class SymTypeChar: public Symbol {
public:
	SymTypeChar() {
		name = "char";
	}
};

class SymTypeFloat: public Symbol {
public:
	SymTypeFloat() {
		name = "float";
	}
};

class SymTypeInteger: public Symbol {
public:
	SymTypeInteger() {
		name = "int";
	}
};

class SymTypeArray: public Symbol {
	Symbol *elemType;
	int length;

public:
	
};

class SymTypeStruct: public Symbol {

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
	}
};

//--- SymFunction ---

class SymFunc: public Symbol {
public:

};

//--- SymTable ---

class SymTable {
	map<string, Symbol*> *var, *type;
	map<string, SymConst*> *c;

public:
	SymTable();

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
	void Pop();
	void Print();
	SymTable* GetTopTable();
};

#endif SYM_TABLE_H
