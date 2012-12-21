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
	map<string, Symbol*> table;

public:
	SymTable() {}

	bool At(string name) {
		return table[name] == NULL ? false : true;
	}

	Symbol* Find(string name) {
		return table[name];
	}

	void Add(string name, Symbol *symb) {
		table[name] = symb;
	}
};

class SymTableStack: public SymTable {
	class SymNode {
		SymNode *next;
		SymTable *table;

	public:
		SymNode(SymNode *_next, SymTable *_table) {
			next = _next;
			table = _table;
		}

		SymTable* GetTable() {
			return table;
		}

		SymNode* GetNext() {
			return next;
		}
	};

	SymNode *top;

public:
	SymTableStack() {
		top = NULL;
	}

	void Add(SymTable *st) {
		SymNode *sn = new SymNode(top, st);
		top = sn;
	}

	SymTable* GetTopTable() {
		return top->GetTable();
	}

	void DeleteTop() {
		SymNode *sn = top;
		top = sn->GetNext();
		delete sn;
	}
};

#endif SYM_TABLE_H
