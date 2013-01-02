#include "node.h"

void Node::DrawPath(int i, bool b) {
	string str = b ? "|   " : "    ";
		
	for(int j = 0; j < i - 1; j++) cout << "    ";
	for(int j = i <= 0 ? 0 : i - 1; j < i; j++) cout << str;
	cout << "|" << endl;
	for(int j = 0; j < i - 1; j++) cout << "    ";
	for(int j = i <= 0 ? 0 : i - 1; j < i; j++) cout << str;
	cout << "+---";
}