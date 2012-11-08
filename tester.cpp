#include "scanner.h"
#include "scanner.cpp"
#include "token.h"
#include "token.cpp"

using namespace std;

bool CompareFiles(const char *fileOut, const char *fileTestOut) {
	ifstream out, testOut;
	string str1, str2;

	out.open(fileOut, ios::in);
	testOut.open(fileTestOut, ios::in);

	while(!out.eof() || !testOut.eof()) {
		getline(out, str1);
		getline(testOut, str2);
		if(str1 != str2)
			return false;
	}
	return true;
}

void test(int i, bool exp) {
	if(exp) cout << "Test " << i << " successful" << endl;
	else	cout << "Test " << i << " error" << endl;
}

int main() {
	for(int i = 1; i < 10; i++) {
		string str  = string("tests/test") + char(i + 48) + string("Input.txt");
		string str1 = string("tests/test") + char(i + 48) + string("Output.txt");

		const char* s = str.c_str();
		const char* s1 = str1.c_str();

		Scanner scanner(s);
		test(i, CompareFiles("output", s1));
	}

	system("PAUSE");
	return 0;
}
