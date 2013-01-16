#include "sym_table.h"

//--- SymTable ---

SymTable::SymTable() {
	var = new map<string, Symbol*>();
	c = new map<string, SymConst*>();
	type = new map<string, Symbol*>();
}

void SymTable::Print() {
	cout << endl << "data segment" << endl;
	if( !c->empty() ) {
		for(map<string, SymConst*>::iterator it = c->begin(); it != c->end(); it++) {
			cout << "\t" << it->first << "\t\tdb\t'" << it->second->GetConst() << "', 0" << endl;
		}
	}
	if( !var->empty() ) {
		for(map<string, Symbol*>::iterator it = var->begin(); it != var->end(); it++) {
			if(it->second->GetSymType() != FUNCTION)
				cout << "\t" << it->first << "\t\tdd\t ?" << endl;
		}
	}
	cout << endl << "data ends" << endl << endl;
}

map<string, Symbol*>* SymTable::GetTableVar() {
	return var;
}

map<string, SymConst*>* SymTable::GetTableConst() {
	return c;
}

map<string, Symbol*>* SymTable::GetTableType() {
	return type;
}

bool SymTable::VarAt(string name) {
	return var->count(name);
}

bool SymTable::ConstAt(string name) {
	return c->count(name);
}

bool SymTable::TypeAt(string name) {
	return type->count(name);
}

Symbol* SymTable::FindVar(string name) {
	if( var->count(name) )
		return (*var)[name];

	return NULL;
}

SymConst* SymTable::FindConst(string name) {
	if( c->count(name) )
		return (*c)[name];

	return NULL;
}

Symbol* SymTable::FindType(string name) {
	if( type->count(name) )
		return (*type)[name];

	return NULL;
}
	
void SymTable::AddVar(Symbol *symb) {
	(*var)[symb->GetName()] = symb;
}

void SymTable::AddConst(SymConst *symb) {
	(*c)[symb->GetName()] = symb;
}

void SymTable::AddType(Symbol *symb) {
	(*type)[symb->GetName()] = symb;
}

//--- SymTableStack ---

SymTableStack::SymTableStack() {
	top = NULL;
	Push(new SymTable());
}

void SymTableStack::Push(SymTable *st) {
	SymNode *sn = new SymNode(top, st);
	top = sn;
}

SymTable* SymTableStack::Pop() {
	SymTable *t = top->GetTable();
	SymNode *sn = top;
	top = sn->GetNext();
	return t;
}
	
SymTable* SymTableStack::GetTopTable() {
	return top->GetTable();
}

void SymTableStack::Print() {
	SymNode *sn = top;
	while(sn != NULL) {
		sn->GetTable()->Print();
		sn = sn->GetNext();
	}
}

bool SymTableStack::VarAt(string name) {
	SymNode *sn = top;
	while(sn != NULL) {
		if(sn->GetTable()->VarAt(name))
			return true;

		sn = sn->GetNext();
	}
	return false;
}

bool SymTableStack::ConstAt(string name) {
	SymNode *sn = top;
	while(sn != NULL) {
		if(sn->GetTable()->ConstAt(name))
			return true;

		sn = sn->GetNext();
	}
	return false;
}

bool SymTableStack::TypeAt(string name) {
	SymNode *sn = top;
	while(sn != NULL) {
		if(sn->GetTable()->TypeAt(name))
			return true;

		sn = sn->GetNext();
	}
	return false;
}

Symbol* SymTableStack::FindVar(string name) {
	SymNode *sn = top;
	while(sn != NULL) {
		if(sn->GetTable()->VarAt(name))
			return sn->GetTable()->FindVar(name);

		sn = sn->GetNext();
	}
	return NULL;
}

Symbol* SymTableStack::FindType(string name) {
	SymNode *sn = top;
	while(sn != NULL) {
		if(sn->GetTable()->TypeAt(name))
			return sn->GetTable()->FindType(name);

		sn = sn->GetNext();
	}
	return NULL;
}

SymConst* SymTableStack::FindConst(string name) {
	SymNode *sn = top;
	while(sn != NULL) {
		if(sn->GetTable()->ConstAt(name))
			return sn->GetTable()->FindConst(name);

		sn = sn->GetNext();
	}
	return NULL;
}

void SymTableStack::AddVar(Symbol *sym) {
	top->GetTable()->AddVar(sym);
}

void SymTableStack::AddConst(SymConst *sym) {
	top->GetTable()->AddConst(sym);
}

void SymTableStack::AddType(Symbol *sym) {
	top->GetTable()->AddType(sym);
}