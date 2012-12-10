#ifndef NODE_H
#define NODE_H

class Node {
public:
	Node *left, *right;
	Token *oper;

	Node(Node *_left, Token *_oper, Node *right) {
		left = _left;
		right = _right;
		oper = _oper;
	}
};

#endif NODE_H