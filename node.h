#ifndef NODE_H
#define NODE_H

class Node {
protected:
	void DrawPath(int i, bool b) {
		string str = b ? "|   " : "    ";
		
		for(int j = 0; j < i; j++) cout << str;
		cout << "|" << endl;
		for(int j = 0; j < i; j++) cout << str;
		cout << "+---";
	}

public:
	virtual void Print(int i, bool b) {}
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
};

class NodeCall: public NodeVar {
	Node *args;

public:
	NodeCall(string _name, Node *_args): NodeVar(_name) {
		args = _args;
	}

	void Print(int i, bool b) { //remake
		cout << "(";
		args->Print(i + 1, true);
		cout << ")";
	}
};

class NodeUnary: public Node {
protected:
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
};

class NodeStruct: public Node {
	Node *structure, *elem;

public:
	NodeStruct(Node *_structure, Node *_elem) {
		structure = _structure;
		elem = _elem;
	}

	void Print(int i, bool b) {
		structure->Print(i + 1, true);
		
		DrawPath(i, b);
		elem->Print(i + 1, false);
	}
};

class NodeBinary: public NodeUnary {
	Node *left;

public:
	NodeBinary(Node *_left, string _opname, Node *_right): NodeUnary(_opname, _right) {
		left = _left;
	}

	void Print(int i, bool b) {
		cout << "(" << opname << ")" << endl;

		DrawPath(i, b);
		left->Print(i + 1, true);

		DrawPath(i, b);
		right->Print(i + 1, false);
	}
};

class NodeFunc: public Node {

};

//--- Statement ---

class NodeCompoundStmt: public Node {
public:

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

class NodeIterationStmt: public Node {

public:

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
public:

};

class NodeFuncDefStmt: public Node {
public:

};

//--- Declaration ---

class NodeDeclaration: public Node {

public:

};

#endif NODE_H