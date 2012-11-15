#include "scanner.h"
#include <io.h>

bool FileExists(const char *fname) {
	return _access(fname, 0) != -1;
}

void GetHelp() {
	cout << "Scanner. Author: Krilov Alexey, C8303A" << endl;
	cout << "L <filename> - lexical analysis" << endl;
	cout << "S <filename> - syntax analysis" << endl;
	cout << "H - help" << endl << endl;
}

int main(int argc, char *argv[]) {
	switch(argc) {
		case 1: {
			GetHelp();
			break;
		}
		case 2: {
			if(argv[1][0] == 'H')
				GetHelp();
			else
				cout << "Error: Invalid argument" << endl;
			break;
		}
		case 3: {
			if(FileExists(argv[2])) {
				if(argv[1][0] == 'L') {
					Scanner scanner(argv[2]);
					cout << "Analysis is completed. Result saved in \"output.txt\"" << endl;
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
