#include "scanner.h"
#include <io.h>

bool FileExists(const char *fname) {
	return _access(fname, 0) != -1;
}

void GetHelp() {
	cout << "Scanner. Author: Krilov Alexey, C8303A" << endl;
	cout << "L <filename> - lexical analysis" << endl;
	cout << "S <filename> - syntax analysis" << endl << endl;
}

int main() {
	char filename[100];

	GetHelp();
	cin >> filename;
		
	if(FileExists(filename)) {
		Scanner scanner(filename);
		cout << "Analysis is completed" << endl;
	}
	else
		cout << "File is not found" << endl;

	return 0;
}
