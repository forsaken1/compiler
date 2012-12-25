#include "generator.h"
#include <io.h>

bool FileExists(const char *fname) {
	return _access(fname, 0) != -1;
}

void GetHelp() {
	cout << "Compilator C. Author: Krilov Alexey, C8303A" << endl;
	cout << "l <filename> - lexical analysis" << endl;
	cout << "s <filename> - syntax analysis" << endl;
	cout << "c <filename> - compilation" << endl;
	cout << "h - help" << endl << endl;
}

int main(int argc, char *argv[]) {
	try {
		switch(argc) {
			case 1: {
				//GetHelp();
				Scanner scanner("input.txt"); //for debug
				Parser parser(&scanner, false, false);
				Generator gen(&parser);
				//cout << endl;
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
					if(argv[1][0] == 'L') {
						Scanner scanner(argv[2]);
						scanner.Start();
					}
					if(argv[1][0] == 'l') {
						Scanner scanner(argv[2]);
						scanner.Start();
						cout << endl << "Lexical analysis is completed." << endl;
					}
					if(argv[1][0] == 'S') {
						Scanner scanner(argv[2]);
						Parser parser(&scanner, true, true);
					}
					if(argv[1][0] == 's') {
						Scanner scanner(argv[2]);
						Parser parser(&scanner, false, false);
						cout << endl << "Syntax analysis is completed." << endl;
					}
					if(argv[1][0] == 'C' || argv[1][0] == 'c') {
						Scanner scanner(argv[2]);
						Parser parser(&scanner, false, false);
						cout << endl << "Compilation is completed." << endl;
					}
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
	//system("PAUSE");
	return 0;
}
