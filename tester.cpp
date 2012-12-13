#include "scanner.h"
#include "parser.h"

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
	/*int FROM = 1;
	int TO	 = 60;
	for(int i = FROM; i <= TO; i++) { 
		string str  = string("tests/scanner/") + IntToStr(i) + string(".in");
		string str1 = string("tests/scanner/") + IntToStr(i) + string(".out");

		const char* s = str.c_str();
		const char* s1 = str1.c_str();

		Scanner scanner(s);
		scanner.Start();
		test(i, CompareFiles("output.txt", s1));
	}*/
	int FROM = 1;
	int TO	 = 20;
	for(int i = FROM; i <= TO; i++) { 
		string str  = string("tests/parser/") + IntToStr(i) + string(".in");
		string str1 = string("tests/parser/") + IntToStr(i) + string(".out");

		const char* s = str.c_str();
		const char* s1 = str1.c_str();
		
		freopen("output.txt", "w", stdout);
		Scanner scanner(s);
		Parser parser(&scanner);
		freopen("CON", "w", stdout);
		test(i, CompareFiles("output.txt", s1));
	}
	system("PAUSE");
	return 0;
}
