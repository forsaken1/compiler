#include "sym_table.h"

//--- SymTable ---

SymTable::SymTable() {
	var = new map<string, Symbol*>();
	c = new map<string, SymConst*>();
	type = new map<string, Symbol*>();
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
	return (*var)[name] == NULL ? false : true;
}

bool SymTable::ConstAt(string name) {
	return (*c)[name] == NULL ? false : true;
}

bool SymTable::TypeAt(string name) {
	return (*type)[name] == NULL ? false : true;
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

void SymTableStack::Pop() {
	SymNode *sn = top;
	top = sn->GetNext();
}
	
SymTable* SymTableStack::GetTopTable() {
	return top->GetTable();
}

void SymTableStack::Print() {
	SymNode *sn = top;
	while(sn != NULL) {
		if( !sn->GetTable()->GetTableConst()->empty() ) {
			for(map<string, SymConst*>::iterator it = sn->GetTable()->GetTableConst()->begin(); 
			it != sn->GetTable()->GetTableConst()->end(); it++) {
				cout << "\t" << it->first << "\t\tdb\t'" << it->second->GetConst() << "', 0" << endl;
			}
		}
		if( !sn->GetTable()->GetTableVar()->empty() ) {
			for(map<string, Symbol*>::iterator it = sn->GetTable()->GetTableVar()->begin(); 
			it != sn->GetTable()->GetTableVar()->end(); it++) {
				cout << "\t" << it->first << "\t\tdd\t ?" << endl;
			}
		}
		sn = sn->GetNext();
	}
}