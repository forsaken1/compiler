#include "scanner.h"
#include "parser.h"
#include <io.h>

bool FileExists(const char *fname) {
	return _access(fname, 0) != -1;
}

void GetHelp() {
	cout << "Compilator C. Author: Krilov Alexey, C8303A" << endl;
	cout << "L <filename> - lexical analysis" << endl;
	cout << "S <filename> - syntax analysis" << endl;
	cout << "H - help" << endl << endl;
}

int main(int argc, char *argv[]) {
	switch(argc) {
		case 1: {
			//GetHelp();
			Scanner scanner("input.txt");
			Parser parser(&scanner);
			cout << endl;
			system("PAUSE");
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
					cout << "Lexical analysis is completed." << endl;
				}
				if(argv[1][0] == 'S' || argv[1][0] == 's') {
					Scanner scanner(argv[2]);
					Parser parser(&scanner);
					cout << endl << "Syntax analysis is completed." << endl;
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
	return 0;
}
