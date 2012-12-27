#ifndef GENERATOR_H
#define GENERATOR_H

#include "parser.h"

class Generator {
	Parser *pr;

public:
	Generator(Parser *_pr) {
		pr = _pr;

		freopen("out.asm", "w", stdout);

		cout << ".686" << endl << ".model flat, stdcall" << endl << endl;
		cout << "include \\masm32\\include\\masm32rt.inc" << endl << endl;
		cout << ".data" << endl;

		pr->GetSymStack()->Print();

		cout << endl << ".code" << endl << "start:" << endl;

		CodeGen *cg = new CodeGen();
		pr->GetTop()->Generate(cg);

		cout << "\texit" << endl;
		cout << endl << "end start" << endl;
		
		freopen("log.txt", "w", stdout);
		system("make.bat");
		freopen("CON", "w", stdout);
	}
};

#endif GENERATOR_H