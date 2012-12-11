#ifndef NODE_H
#define NODE_H

class Node {};

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
}

class NodeBinary: public NodeUnary {
public:
	Node *left;

	NodeBinary(Node *_left, string _opname, Node *_right): NodeUnary(_opname, _right) {
		left = _left;
	}
}

class NodeFunc: public Node {

};

#endif NODE_H