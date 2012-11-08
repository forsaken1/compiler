#include "scanner.h"

int main() {
	char filename[100];
	cout << "Scanner. Author: Krilov Alexey, C8303A" << endl << endl;
	cout << "Enter filename for lexical analysis: ";
	cin >> filename;
	Scanner scanner(filename);
	return 0;
}
