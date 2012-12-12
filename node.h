#ifndef NODE_H
#define NODE_H

class Node {
public:
	virtual void Print() {}
};

//--- Expression ---

class NodeConst: public Node {
public:
	string constant;

	NodeConst(string _constant) {
		constant = _constant;
	}

	void Print() {
		cout << "(" << constant << ")";
	}
};

class NodeVar: public Node {
public:
	string name;

	NodeVar(string _name) {
		name = _name;
	}

	void Print() {
		cout << "(" << name << ")";
	}
};

class NodeCall: public NodeVar {
public:
	Node *args;

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
public:
	string opname;
	Node *right;

	NodeUnary(string _opname, Node *_right) {
		opname = _opname;
		right = _right;
	}

	void Print() {
		cout << "[ " << opname << " ";
		right->Print();
		cout << " ]";
	}
};

class NodeUnaryPostfix: public NodeUnary {
public:
	NodeUnaryPostfix(string _opname, Node *_right): NodeUnary(_opname, _right) {}

	void Print() {
		cout << "[ (" << opname << ") ";
		right->Print();
		cout << " ]";
	}
};

class NodeBinary: public NodeUnary {
public:
	Node *left;

	NodeBinary(Node *_left, string _opname, Node *_right): NodeUnary(_opname, _right) {
		left = _left;
	}

	void Print() {
		cout << "[ ";
		left->Print();
		cout << " " << opname << " ";
		right->Print();
		cout << " ]";
	}
};

class NodeTernary: public Node {
public:
	Node *condition, *cond_true, *cond_false;

	NodeTernary(Node *_condition, Node *_cond_true, Node *_cond_false) {
		condition = _condition;
		cond_true = _cond_true;
		cond_false = _cond_false;
	}

	void Print() {
		cout << "[ ";
		condition->Print();
		cout << " ? ";
		cond_true->Print();
		cout << " : ";
		cond_false->Print();
		cout << " ]";
	}
};

class NodeFunc: public Node {

};

//--- Statement ---

class NodeCompoundStmt: public Node {
public:

};

class NodeExpressionStmt: public Node {
public:
	Node *expr;

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
public:

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