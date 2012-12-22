#ifndef NODE_H
#define NODE_H

#include "sym_table.h"

class Node {
protected:
	void DrawPath(int i, bool b) {
		string str = b ? "|   " : "    ";
		
		for(int j = 0; j < i - 1; j++) cout << "    ";
		for(int j = i <= 0 ? 0 : i - 1; j < i; j++) cout << str;
		cout << "|" << endl;
		for(int j = 0; j < i - 1; j++) cout << "    ";
		for(int j = i <= 0 ? 0 : i - 1; j < i; j++) cout << str;
		cout << "+---";
	}

public:
	virtual void Print(int i, bool b) {}
	Symbol* GetType(SymTable *symTable) { return NULL; }
	string GetName() {}
	void Generate() {}
};

//--- Expression ---

class NodeConst: public Node {
	string constant;

public:
	NodeConst(string _constant) {
		constant = _constant;
	}

	void Print(int i, bool b) {
		cout << "(" << constant << ")" << endl;
	}
};

class NodeVar: public Node {
	string name;

public:
	NodeVar(string _name) {
		name = _name;
	}

	void Print(int i, bool b) {
		cout << "(" << name << ")" << endl;
	}

	Symbol* GetType(SymTable *symTable) {
		return symTable->Find(name);
	}
};

class NodeCall: public NodeVar {
	Node *args;

public:
	NodeCall(string _name, Node *_args): NodeVar(_name) {
		args = _args;
	}

	void Print(int i, bool b) { 
		cout << "(";
		args->Print(i + 1, true);
		cout << ")";
	}

	Symbol* GetType() {}
};

class NodeUnary: public Node {
	string opname;
	Node *right;

public:
	NodeUnary(string _opname, Node *_right) {
		opname = _opname;
		right = _right;
	}

	void Print(int i, bool b) {
		cout << "(" << opname << ")" << endl;

		DrawPath(i, b);
		right->Print(i + 1, false);
	}

	Symbol* GetType(SymTable *symTable) {
		return right->GetType(symTable);
	}
};

class NodeStruct: public Node {
	Node *elem;
	string name;

public:
	NodeStruct(string _name, Node *_elem) {
		name = _name;
		elem = _elem;
	}

	string GetName() {
		return name;
	}

	void Print(int i, bool b) {
		cout << "struct (" << name << ")" << endl;
		
		if(elem != NULL) {
			DrawPath(i, b);
			elem->Print(i + 1, false);
		}
	}
};

class NodeBinary: public Node {
	string opname;
	Node *right;
	Node *left;

public:
	NodeBinary(Node *_left, string _opname, Node *_right) {
		left = _left;
		opname = _opname;
		right = _right;
	}

	void Print(int i, bool b) {
		cout << "(" << opname << ")" << endl;

		DrawPath(i, b);
		left->Print(i + 1, true);

		DrawPath(i, b);
		right->Print(i + 1, false);
	}

	Symbol* GetType(SymTable *symTable) {
		if(left->GetType(symTable) == right->GetType(symTable))
			return left->GetType(symTable);

		return NULL;
	}
};

class NodeFunc: public Node {
	Symbol *returnValue;
	Node *stmt, *args, *name;

public:
	NodeFunc(Symbol *_returnValue, Node *_name, Node *_args, Node *_stmt) {
		returnValue = _returnValue;
		stmt = _stmt;
		name = _name;
		args = _args;
	}

	void Print(int i, bool b) {
		cout << returnValue->GetName() << " function ";
		name->Print(i + 1, true);

		if(args != NULL) {
			DrawPath(i, b);
			args->Print(i + 1, stmt == NULL ? false : true);
		}

		if(stmt != NULL) {
			DrawPath(i, b);
			stmt->Print(i + 1, false);
		}
	}

	Symbol* GetType() {
		return returnValue;
	}
};

class NodePrint: public Node {
	Node *expr;

public:
	NodePrint(Node *_expr) {
		expr = _expr;
	}

	void Print(int i, bool b) {
		cout << "print" << endl;
		DrawPath(i, b);
		expr->Print(i + 1, false);
	}
};

//--- Statement ---

class NodeStmt: public Node {
	Node *first, *second;
	string name;

public:
	NodeStmt(string _name, Node *_first, Node *_second) {
		name = _name;
		first = _first;
		second = _second;
	}

	void Print(int i, bool b) {
		cout << name << endl;
		if(first != NULL) {
			DrawPath(i, b);
			first->Print(i + 1, second == NULL ? false : true);
		}
		if(second != NULL) {
			DrawPath(i, b);
			second->Print(i + 1, false);
		}
	}
};

class NodeExpressionStmt: public Node {
	Node *expr;

public:
	NodeExpressionStmt(Node *_expr) {
		expr = _expr;
	}

	void Print(int i, bool b) {
		expr->Print(i + 1, true);
	}
};

class NodeIterationWhile: public Node {
protected:
	Node *cond, *stmt;

public:
	NodeIterationWhile(Node *_cond, Node *_stmt) {
		cond = _cond;
		stmt = _stmt;
	}

	void Print(int i, bool b) {
		cout << "while" << endl;

		DrawPath(i, b);
		cond->Print(i + 1, true);

		DrawPath(i, b);
		stmt->Print(i + 1, false);
	}
};

class NodeIterationDo: public NodeIterationWhile {

public:
	NodeIterationDo(Node *_cond, Node *_stmt): NodeIterationWhile(_cond, _stmt) {}

	void Print(int i, bool b) {
		cout << "do" << endl;
		DrawPath(i, b);
		cond->Print(i + 1, true);

		DrawPath(i, b);
		stmt->Print(i + 1, false);
	}
};

class NodeIterationFor: public NodeIterationWhile {
	Node *iter, *init;

public:
	NodeIterationFor(Node *_init, Node *_cond, Node *_iter, Node *_stmt): NodeIterationWhile(_cond, _stmt) {
		iter = _iter;
		init = _init;
	}

	void Print(int i, bool b) {
		cout << "for" << endl;

		if(init != NULL) {
			DrawPath(i, b);
			init->Print(i + 1, true);
		}

		if(cond != NULL) {
			DrawPath(i, b);
			cond->Print(i + 1, true);
		}

		if(iter != NULL) {
			DrawPath(i, b);
			iter->Print(i + 1, true);
		}

		DrawPath(i, b);
		stmt->Print(i + 1, false);
	}
};

class NodeSelectionStmt: public Node {
	Node *expr, *trueStmt, *falseStmt;

public:
	NodeSelectionStmt(Node *_expr, Node *_trueStmt, Node *_falseStmt) {
		expr = _expr;
		trueStmt = _trueStmt;
		falseStmt = _falseStmt;
	}

	void Print(int i, bool b) {
		cout << "if" << endl;
		DrawPath(i, b);
		expr->Print(i + 1, true);

		DrawPath(i, b);
		trueStmt->Print(i + 1, falseStmt == NULL ? false : true);

		if(falseStmt != NULL) {
			DrawPath(i, b);
			falseStmt->Print(i + 1, false);
		}
	}
};

class NodeJumpStmt: public Node {
	string name;
	Node *expr;

public:
	NodeJumpStmt(string _name, Node *_expr) {
		name = _name;
		expr = _expr;
	}

	void Print(int i, bool b) {
		cout << "(" << name << ")" << endl;
		if(expr != NULL) {
			DrawPath(i, b);
			expr->Print(i + 1, b);
		}
	}
};

//--- Declaration ---

class NodeDeclaration: public Node {

public:

};

#endif NODE_H