#ifndef NODE_H
#define NODE_H

class Node {
public:
	virtual void Print(int i) {}
};

//--- Expression ---

class NodeConst: public Node {
	string constant;

public:
	NodeConst(string _constant) {
		constant = _constant;
	}

	void Print(int i) {
		cout << "(" << constant << ")" << endl;
	}
};

class NodeVar: public Node {
	string name;

public:
	NodeVar(string _name) {
		name = _name;
	}

	void Print(int i) {
		cout << "(" << name << ")" << endl;
	}
};

class NodeCall: public NodeVar {
	Node *args;

public:
	NodeCall(string _name, Node *_args): NodeVar(_name) {
		args = _args;
	}

	void Print(int i) {
		cout << "(";
		args->Print(i + 1);
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

	void Print(int i) {
		cout << "{ (" << opname << ") ";
		right->Print(i + 1);
		cout << " }";
	}
};

class NodeUnaryPostfix: public NodeUnary {
public:
	NodeUnaryPostfix(string _opname, Node *_right): NodeUnary(_opname, _right) {}

	void Print(int i) {
		cout << "{ ";
		right->Print(i + 1);
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

	void Print(int i) {
		cout << "{ ";
		structure->Print(i + 1);
		cout << " [ ";
		elem->Print(i + 1);
		cout  << " ] }";
	}
};

class NodeBinary: public NodeUnary {
	Node *left;

public:
	NodeBinary(Node *_left, string _opname, Node *_right): NodeUnary(_opname, _right) {
		left = _left;
	}

	void Print(int i) {
		cout << "(" << opname << ")" << endl;

		for(int j = 0; j < i; j++)
			cout << "|   ";

		cout << "|" << endl;

		for(int j = 0; j < i; j++)
			cout << "+   ";

		cout << "+---";
		
		left->Print(i + 1);

		for(int j = 0; j < i; j++)
			cout << "|   ";
		
		cout << "|" << endl;

		for(int j = 0; j < i; j++)
			cout << "+   ";

		cout << "+---";

		right->Print(i + 1);
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

	void Print(int i) {
		cout << "{ ";
		condition->Print(i + 1);
		cout << " ? ";
		cond_true->Print(i + 1);
		cout << " : ";
		cond_false->Print(i + 1);
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

	void Print(int i) {
		expr->Print(i + 1);
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

	void Print(int i) {
		cout << "if ( ";
		expr->Print(i + 1);
		cout << " ) { ";
		trueStmt->Print(i + 1);
		cout << " } ";

		if(falseStmt != NULL) {
			cout << "else { ";
			falseStmt->Print(i + 1);
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

public:

};

#endif NODE_H