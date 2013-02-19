#ifndef GENERATOR_H
#define GENERATOR_H

#include "parser.h"

class Generator {
public:
	Generator(Parser *pr, bool optimisation) {
		CodeGen *cg = new CodeGen(optimisation);

		freopen("out.asm", "w", stdout);

		cout << ".686" << endl << ".model flat, stdcall" << endl << endl;
		cout << "include \\masm32\\include\\masm32rt.inc" << endl << endl;

		pr->GetSymStack()->Print();

		cout << endl << ".code" << endl << "start:" << endl;

		cg->AddCommand(CALL, "_main");
		cg->AddCommand(EXIT);
		pr->GetTop()->Generate(cg);
		cg->Print();

		cout << endl << "end start" << endl;
		
		freopen("log.txt", "w", stdout);
		system("make.bat");
		freopen("CON", "w", stdout);
		//system("del log.txt");
	}
};

#endif GENERATOR_H