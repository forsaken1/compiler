#ifndef SYM_TABLE_H
#define SYM_TABLE_H

//#include <map>

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
	map<string, Symbol*> *var, *c, *type;

public:
	SymTable() {
		var = new map<string, Symbol*>();
		c = new map<string, Symbol*>();
		type = new map<string, Symbol*>();
	}

	map<string, Symbol*>* GetTableVar() {
		return var;
	}

	map<string, Symbol*>* GetTableConst() {
		return c;
	}

	map<string, Symbol*>* GetTableType() {
		return type;
	}

	bool VarAt(string name) {
		return (*var)[name] == NULL ? false : true;
	}

	bool ConstAt(string name) {
		return (*c)[name] == NULL ? false : true;
	}

	bool TypeAt(string name) {
		return (*type)[name] == NULL ? false : true;
	}

	Symbol* FindVar(string name) {
		return (*var)[name];
	}

	Symbol* FindConst(string name) {
		return (*c)[name];
	}

	Symbol* FindType(string name) {
		return (*type)[name];
	}
	
	void AddVar(Symbol *symb) {
		(*var)[symb->GetName()] = symb;
	}

	void AddConst(Symbol *symb) {
		(*c)[symb->GetName()] = symb;
	}

	void AddType(Symbol *symb) {
		(*type)[symb->GetName()] = symb;
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
		Push(new SymTable());
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

	void Print() {
		SymNode *sn = top;
		while(sn != NULL) {
			if( !sn->GetTable()->GetTableConst()->empty() ) {
				//for(map<string, Symbol*>::iterator it = sn->GetTable()->GetTableConst().begin(); 
				//it != sn->GetTable()->GetTableConst().end(); it++) {
					//cout << "\t" << "const_" << (*it).first << "\t\tdb\t'" << (*it).first << "', 0" << endl;
				map<string, Symbol*>::iterator it;
				it = sn->GetTable()->GetTableConst()->begin();
				//it++;
				cout << "\t" << "const_" << it->first << "\t\tdb\t'" << it->first << "', 0" << endl;
				//}
			}
			sn = sn->GetNext();
		}
	}
};

#endif SYM_TABLE_H
