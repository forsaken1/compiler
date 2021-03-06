#include <io.h>
#include <time.h>
#include "generator.h"

bool FileExists(const char *fname) {
	return _access(fname, 0) != -1;
}

void GetHelp() {
	cout << "Compiler C. Author: Krylov Alexey, C8303A" << endl;
	cout << "(L | l) <filename> - lexical analysis" << endl;
	cout << "(S | s) <filename> - (syntax | semantic) analysis" << endl;
	cout << "(C | c) <filename> - (without | with optimisation) compilation" << endl;
	cout << "h - help" << endl << endl;
}

int main(int argc, char *argv[]) {
	srand(static_cast<unsigned>(time(NULL)));
	try {
		switch(argc) {
			case 1: {
				//GetHelp();
				/*Scanner scanner("input.txt");
				Parser parser(&scanner, 1, 1); //no semantics, printing tree
				Generator gen(&parser, 0);*/
				break;
			}
			case 2: {
				if(argv[1][0] == 'H' || argv[1][0] == 'h')
					GetHelp();
				else
					cout << "Error: Invalid argument" << endl;
				break;
			}
			case 3: {
				if(FileExists(argv[2])) {
					if(argv[1][0] == 'L' || argv[1][0] == 'l') {
						Scanner scanner(argv[2]);
						scanner.Start();
					}
					else if(argv[1][0] == 'S') { //without semantics
						Scanner scanner(argv[2]);
						Parser parser(&scanner, true, true);
					} 
					else if(argv[1][0] == 's') { //with semantics
						Scanner scanner(argv[2]);
						Parser parser(&scanner, false, true);
					}
					else if(argv[1][0] == 'C') { //without optimisation
						Scanner scanner(argv[2]);
						Parser parser(&scanner, false, false);
						Generator gen(&parser, false);
					}
					else if(argv[1][0] == 'c') { //with optimisation
						Scanner scanner(argv[2]);
						Parser parser(&scanner, false, false);
						Generator gen(&parser, true);
					}
					else
						cout << "Error: Invalid argument" << endl;
				}
				else
					cout << "Error: File is not found" << endl;
				break;
			}
			default: {
				cout << "Error: Too many arguments" << endl;
			}
		}
	}
	catch(Exception &e) {
		cout << e.GetMessage() << endl;
	}
	return 0;
}
