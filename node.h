#ifndef NODE_H
#define NODE_H

#include "scanner.h"
#include "sym_table.h"
#include "codegen.h"

//--- Node ---

class Node {
protected:
	void DrawPath(int, bool);

public:
	virtual void Print(int i, bool b) {}
	virtual void Generate(CodeGen *cg) {}
	virtual string GetId() { return ""; }
	virtual string GetName() { return ""; }

	Symbol* GetType(SymTable *symTable) { return NULL; }
};

//--- NodeConst ---

class NodeConst: public Node {
	string constant, id;

public:
	NodeConst(string, string);

	void Print(int, bool);
	void Generate(CodeGen*);
	string GetId();
};

//--- NodeVar ---

class NodeVar: public Node {
	string name;

public:
	NodeVar(string);

	void Print(int, bool);
	void Generate(CodeGen*);
	Symbol* GetType(SymTable*);
	string GetName();
};

//--- NodeCall ---

class NodeCall: public NodeVar {
	Node *args;

public:
	NodeCall(string, Node*);

	void Print(int, bool);
};

//--- NodeStruct ---

class NodeStruct: public Node {
	Node *elem;
	string name;

public:
	NodeStruct(string, Node*);
	
	void Print(int, bool);
	string GetName();
};

//--- NodeUnary ---

class NodeUnary: public Node {
	string opname;
	TokenValue opval;
	Node *right;

	void Dec(CodeGen*);
	void Inc(CodeGen*);
	void Dereference(CodeGen*);
	void Plus(CodeGen*);
	void Minus(CodeGen*);
	void BinaryNot(CodeGen*);
	void Not(CodeGen*);

	string GetOpName(TokenValue tv);

public:
	NodeUnary(TokenValue, Node*);

	void Print(int, bool);
	void Generate(CodeGen*);
	Symbol* GetType(SymTable*);
};

//--- NodeBinary ---

class NodeBinary: public Node {
	string opname;
	TokenValue opval;
	Node *right;
	Node *left;

	void Comma(CodeGen*);
	void Assign(CodeGen*);
	void Sub(CodeGen*);
	void Mul(CodeGen*);
	void Div(CodeGen*);
	void Add(CodeGen*);
	void DivideByMod(CodeGen*);
	void And(CodeGen*);
	void Or(CodeGen*);
	void Xor(CodeGen*);
	void ShiftLeft(CodeGen*);
	void ShiftRight(CodeGen*);
	void More(CodeGen*);
	void Less(CodeGen*);
	void Equal(CodeGen*);
	void NotEqual(CodeGen*);

	void PlusAssign(CodeGen*);
	void MinusAssign(CodeGen*);
	void MultiplyAssign(CodeGen*);
	void DivideAssign(CodeGen*);
	void DivideByModAssign(CodeGen*);
	void ExclusiveOrAssign(CodeGen*);

	string GetOpName(TokenValue tv);

public:
	NodeBinary(Node*, TokenValue, Node*);

	void Print(int, bool);
	void Generate(CodeGen*);
	Symbol* GetType(SymTable*);
};

//--- NodeFunc ---

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

//--- NodePrint ---

class NodePrint: public Node {
	Node *expr, *format;

public:
	NodePrint(Node *_format, Node *_expr) {
		format = _format;
		expr = _expr;
	}

	void Print(int i, bool b) {
		cout << "print" << endl;
		DrawPath(i, b);
		format->Print(i + 1, true);
		if(expr != NULL) {
			DrawPath(i, b);
			expr->Print(i + 1, false);
		}
	}

	void Generate(CodeGen *cg) {
		if(expr == NULL) {
			cg->AddCommand(INVOKE, CRT_PRINTF, format->GetId());
		}
		else {
			expr->Generate(cg);
			cg->AddCommand(POP, EAX);
			cg->AddCommand(INVOKE, CRT_PRINTF, format->GetId(), EAX);
		}
	}
};

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
			first->Print(i + 1, second != NULL);
		}
		if(second != NULL) {
			DrawPath(i, b);
			second->Print(i + 1, false);
		}
	}

	void Generate(CodeGen *cg) {
		if(first != NULL)
			first->Generate(cg);

		if(second != NULL)
			second->Generate(cg);
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

#endif NODE_H