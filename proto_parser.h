#ifndef PROTO_PARSER_H
#define PROTO_PARSER_H

#include "scanner.h"

class Node {
public:
	Token *oper;
	Node *left, *right;

	Node(Node *_left, Token *_oper, Node *_right) {
		left = _left;
		right = _right;
		oper = _oper;
	}
};

class ProtoParser {
	Scanner* scanner;
	Token* token;

	Node* ParseExpression();
	Node* ParseFactor();
	Node* ParseTerm();
	bool NextToken();

public:
	ProtoParser(Scanner*);
	void Start();
};

#endif PROTO_PARSER_H