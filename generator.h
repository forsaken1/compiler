#ifndef GENERATOR_H
#define GENERATOR_H

#include "parser.h"

class Generator {
	Parser *pr;

public:
	Generator(Parser *_pr) {
		pr = _pr;

		freopen("result/out.asm", "w", stdout);

		cout << ".686" << endl << ".model flat, stdcall" << endl << endl;
		cout << "include \\masm32\\include\\masm32rt.inc" << endl << endl;
		cout << ".data" << endl;

		pr->GetSymStack()->Print();

		cout << endl << ".code" << endl << "start:" << endl;
		pr->GetTop()->Generate();
		cout << endl << "\tinkey" << endl << "\texit" << endl;
		cout << endl << "end start" << endl;

		fclose(stdout);

		system("result/make.bat");
	}
};

#endif GENERATOR_H