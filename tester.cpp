#include "scanner.h"

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

void MakeFiles(int from, int to) {
	for(int i = from; i <= to; i++) {
		string str  = string("tests/") + IntToStr(i) + string(".in");
		string str1 = string("tests/") + IntToStr(i) + string(".out");
		const char* s = str.c_str();
		const char* s1 = str1.c_str();
		ofstream fo(s, ios::out);
		ofstream fo1(s1, ios::out);
		fo.close();
		fo1.close();
	}
}

int main() {
	int FROM = 52;
	int TO	 = 52;
	for(int i = FROM; i <= TO; i++) { 
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
