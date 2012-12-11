#ifndef NODE_H
#define NODE_H

class Node {};

class NodeConst: public Node {
public:
	string constant;

	NodeConst(string _constant) {
		constant = _constant;
	}
};

class NodeVar: public Node {
public:
	string name;

	NodeVar(string _name) {
		name = _name;
	}
};

class NodeCall: public NodeVar {
public:
	Node *args;

	NodeCall(string _name, Node *_args): NodeVar(_name) {
		args = _args;
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
};

class NodeBinary: public NodeUnary {
public:
	Node *left;

	NodeBinary(Node *_left, string _opname, Node *_right): NodeUnary(_opname, _right) {
		left = _left;
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
};

class NodeFunc: public Node {

};

#endif NODE_H