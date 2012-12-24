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

class SymTypeRecord: public Symbol {

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

	void Add(Symbol *symb) {
		table[symb->GetName()] = symb;
	}
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

	void Push(SymTable *st) {
		SymNode *sn = new SymNode(top, st);
		top = sn;
	}

	void Pop() {
		SymNode *sn = top;
		top = sn->GetNext();
		delete sn;
	}
	
	SymTable* GetTopTable() {
		return top->GetTable();
	}
};

#endif SYM_TABLE_H
