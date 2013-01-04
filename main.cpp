#include <io.h>
#include "generator.cpp"

bool FileExists(const char *fname) {
	return _access(fname, 0) != -1;
}

void GetHelp() {
	cout << "Compiler C. Author: Krylov Alexey, C8303A" << endl;
	cout << "(L | l) <filename> - lexical analysis" << endl;
	cout << "(S | s) <filename> - (syntax | semantic) analysis" << endl;
	cout << "(C | c) <filename> - compilation" << endl;
	cout << "h - help" << endl << endl;
}

int main(int argc, char *argv[]) {
	//cout << (27 >> 1) << endl;
	//system("PAUSE");
	try {
		switch(argc) {
			case 1: {
				//GetHelp();
				Scanner scanner("input.txt");
				Parser parser(&scanner, 0, 0); //no semantics, printing tree
				Generator gen(&parser);
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
					else if(argv[1][0] == 'S') {
						Scanner scanner(argv[2]);
						Parser parser(&scanner, true, true);
					} 
					else if(argv[1][0] == 's') {
						Scanner scanner(argv[2]);
						Parser parser(&scanner, false, true);
					}
					else if(argv[1][0] == 'C' || argv[1][0] == 'c') {
						Scanner scanner(argv[2]);
						Parser parser(&scanner, false, false);
						Generator gen(&parser);
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
