#ifndef NODE_H
#define NODE_H

#include "scanner.h"
#include "sym_table.h"
#include "codegen.h"
#include "lib.h"

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
protected:
	string name;

public:
	NodeVar(string);

	void Print(int, bool);
	virtual void Generate(CodeGen*);
	Symbol* GetType(SymTable*);
	string GetName();
};

//--- NodeCall ---

class NodeCall: public NodeVar {
	Node *args;

public:
	NodeCall(string, Node*);

	void Print(int, bool);
	void Generate(CodeGen*);
};

//--- NodeLocalVar ---

class NodeLocalVar: public NodeVar {
public:
	NodeLocalVar(string _name): NodeVar(_name) {}

	void Generate(CodeGen*);
};

//--- NodeStruct ---

class NodeStruct: public Node {
	Node *elem;
	string name;

public:
	NodeStruct(string, Node*);
	
	void Print(int, bool);
	void Generate(CodeGen*);
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
	void MoreEqual(CodeGen*);
	void LessEqual(CodeGen*);

	void PlusAssign(CodeGen*);
	void MinusAssign(CodeGen*);
	void MultiplyAssign(CodeGen*);
	void DivideAssign(CodeGen*);
	void DivideByModAssign(CodeGen*);
	void ExclusiveOrAssign(CodeGen*);

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
	SymTable *table;

public:
	NodeFunc(SymTable*, Symbol*, Node*, Node*, Node*);

	void Print(int, bool);
	void Generate(CodeGen*);
	Symbol* GetType();
};

//--- NodePrint ---

class NodePrint: public Node {
	Node *expr, *format;
	TokenValue type;
	Cmd func;
	string funcStr, funcStrName;

public:
	NodePrint(TokenValue, Node*, Node*);

	void Print(int, bool);
	void Generate(CodeGen*);
};

//--- NodePause ---

class NodePause: public Node {
public:
	NodePause() {}

	void Generate(CodeGen*);
	void Print(int, bool);
};

//--- NodeSafeStmt ---

class NodeSafeStmt: public Node {
	Node *first, *second;

public:
	NodeSafeStmt(Node*);
	NodeSafeStmt(Node*, Node*);

	void Print(int, bool);
	void Generate(CodeGen*);
};

//--- NodeStmt ---

class NodeStmt: public Node {
	Node *first, *second;
	string name;

public:
	NodeStmt(string, Node*, Node*);

	void Print(int, bool);
	void Generate(CodeGen*);
};

//--- NodeIterationWhile ---

class NodeIterationWhile: public Node {
protected:
	Node *cond, *stmt;

public:
	NodeIterationWhile(Node*, Node*);

	void Print(int, bool);
	void Generate(CodeGen*);
};

//--- NodeIterationDo ---

class NodeIterationDo: public NodeIterationWhile {

public:
	NodeIterationDo(Node*, Node*);

	void Print(int, bool);
	void Generate(CodeGen*);
};

//--- NodeIterationFor ---

class NodeIterationFor: public NodeIterationWhile {
	Node *iter, *init;

public:
	NodeIterationFor(Node*, Node*, Node*, Node*);

	void Print(int, bool);
	void Generate(CodeGen*);
};

//--- NodeSelectionStmt ---

class NodeSelectionStmt: public Node {
	Node *expr, *trueStmt, *falseStmt;

public:
	NodeSelectionStmt(Node*, Node*, Node*);

	void Print(int, bool);
	void Generate(CodeGen*);
};

//--- NodeJumpStmt ---

class NodeJumpStmt: public Node {
	TokenValue tv;
	string name;
	Node *expr;

	void Return(CodeGen*);
	void Continue(CodeGen*);
	void Break(CodeGen*);
	void Goto(CodeGen*);

public:
	NodeJumpStmt(TokenValue, Node*);

	void Print(int, bool);
	void Generate(CodeGen*);
};

#endif NODE_H