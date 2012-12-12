#ifndef NODE_H
#define NODE_H

class Node {
public:
	virtual void Print() {}
};

//--- Expression ---

class NodeConst: public Node {
	string constant;

public:
	NodeConst(string _constant) {
		constant = _constant;
	}

	void Print() {
		cout << "(" << constant << ")";
	}
};

class NodeVar: public Node {
	string name;

public:
	NodeVar(string _name) {
		name = _name;
	}

	void Print() {
		cout << "(" << name << ")";
	}
};

class NodeCall: public NodeVar {
	Node *args;

public:
	NodeCall(string _name, Node *_args): NodeVar(_name) {
		args = _args;
	}

	void Print() {
		cout << "(";
		args->Print();
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

	void Print() {
		cout << "{ (" << opname << ") ";
		right->Print();
		cout << " }";
	}
};

class NodeUnaryPostfix: public NodeUnary {
public:
	NodeUnaryPostfix(string _opname, Node *_right): NodeUnary(_opname, _right) {}

	void Print() {
		cout << "{ ";
		right->Print();
		cout << " (" << opname << ") }";
	}
};

class NodeStruct: public Node {
	Node *structure, *elem;

public:
	NodeStruct(Node *_structure, Node *_elem) {
		structure = _structure;
		elem = _elem;
	}

	void Print() {
		cout << "{ ";
		structure->Print();
		cout << " [ ";
		elem->Print();
		cout  << " ] }";
	}
};

class NodeBinary: public NodeUnary {
	Node *left;

public:
	NodeBinary(Node *_left, string _opname, Node *_right): NodeUnary(_opname, _right) {
		left = _left;
	}

	void Print() {
		cout << "{ ";
		left->Print();
		cout << " " << opname << " ";
		right->Print();
		cout << " }";
	}
};

class NodeTernary: public Node {
	Node *condition, *cond_true, *cond_false;

public:
	NodeTernary(Node *_condition, Node *_cond_true, Node *_cond_false) {
		condition = _condition;
		cond_true = _cond_true;
		cond_false = _cond_false;
	}

	void Print() {
		cout << "{ ";
		condition->Print();
		cout << " ? ";
		cond_true->Print();
		cout << " : ";
		cond_false->Print();
		cout << " }";
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

	void Print() {
		expr->Print();
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

	void Print() {
		cout << "if ( ";
		expr->Print();
		cout << " ) { ";
		trueStmt->Print();
		cout << " } ";

		if(falseStmt != NULL) {
			cout << "else { ";
			falseStmt->Print();
			cout << " }";
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

};

#endif NODE_H