#include "sym_table.h"

//--- SymTable ---

SymTable::SymTable() {
	var = new map<string, Symbol*>();
	c = new map<string, SymConst*>();
	type = new map<string, Symbol*>();
}

void SymTable::Print() {
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
	return (*var)[name] != NULL;
}

bool SymTable::ConstAt(string name) {
	return (*c)[name] != NULL;
}

bool SymTable::TypeAt(string name) {
	return (*type)[name] != NULL;
}

Symbol* SymTable::FindVar(string name) {
	return (*var)[name];
}

SymConst* SymTable::FindConst(string name) {
	return (*c)[name];
}

Symbol* SymTable::FindType(string name) {
	return (*type)[name];
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