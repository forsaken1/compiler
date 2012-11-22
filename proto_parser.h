#ifndef PROTOPARSER_H
#define PROTOPARSER_H

#include "scanner.h"

class ProtoParser {
	Scanner scanner;

	ParseExpression();
	ParseTerm();
	ParseFactor();

public:
	ProtoParser(Scanner&);
	Start();
};

#endif 