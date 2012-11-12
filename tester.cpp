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

	while(!out.eof() && !testOut.eof()) {
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

string IntToStr(int n) {
	if(n < 10) 
		return string(1, n + 48);
	else 
		return string(1, n / 10 + 48) + string(1, n % 10 + 48);
}

int main() {
	int g = 5.435E-2;
	cout << g << endl;
	for(int i = 1; i <= 20; i++) {
		string str  = string("tests/") + IntToStr(i) + string(".in");
		string str1 = string("tests/") + IntToStr(i) + string(".out");

		const char* s = str.c_str();
		const char* s1 = str1.c_str();

		Scanner scanner(s);
		test(i, CompareFiles("output.txt", s1));
	}

	system("PAUSE");
	return 0;
}
