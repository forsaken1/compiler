#include <ios>
#include <string>
#include <fstream>
#include <string.h>
#include <iostream>

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

bool test(int i, bool exp) { 
	if(exp) cout << "Test " << i << " successful" << endl;
	else	cout << "Test " << i << " error" << endl;
	return exp;
}

string IntToStr(int n) {
	if(n > 99) {
		int m = n % 100;
		return string(1, n / 100 + 48) + string(1, m / 10 + 48) + string(1, m % 10 + 48);
	}
	if(n < 10) 
		return string(1, n + 48);
	else 
		return string(1, n / 10 + 48) + string(1, n % 10 + 48);
}

void MakeFiles(string dir, int from, int to) {
	for(int i = from; i <= to; i++) {
		string str  = string("tests/" + dir + "/") + IntToStr(i) + string(".in");
		string str1 = string("tests/" + dir + "/") + IntToStr(i) + string(".out");
		const char* s = str.c_str();
		const char* s1 = str1.c_str();
		ofstream fo(s, ios::out);
		ofstream fo1(s1, ios::out);
		fo.close();
		fo1.close();
	}
}

int main() {
	int ALL_TESTS_SUCCESSFUL = 0;
	/*
	int FROM = 1;
	int TO	 = 60; //60
	for(int i = FROM; i <= TO; i++) { 
		string str  = string("Debug\\compiler L tests\\scanner\\") + IntToStr(i) + string(".in");
		string str1 = string("tests/scanner/") + IntToStr(i) + string(".out");

		const char* s = str.c_str();
		const char* s1 = str1.c_str();
		
		freopen("output.txt", "w", stdout);
		system(s);
		freopen("CON", "w", stdout);
		if( !test(i, CompareFiles("output.txt", s1)) )
			ALL_TESTS_SUCCESSFUL++;
	}
	//*/
	/*
	int _FROM = 1;
	int _TO   = 86; //86
	for(int i = _FROM; i <= _TO; i++) { 
		string str  = string("Debug\\compiler S tests\\parser\\") + IntToStr(i) + string(".in");
		string str1 = string("tests/parser/") + IntToStr(i) + string(".out");

		const char* s = str.c_str();
		const char* s1 = str1.c_str();
		
		freopen("output.txt", "w", stdout);
		system(s);
		freopen("CON", "w", stdout);
		if( !test(i, CompareFiles("output.txt", s1)) )
			ALL_TESTS_SUCCESSFUL++;
	}
	*/
	/*
	int _FROM_ = 1;
	int _TO_   = 1;
	for(int i = _FROM_; i <= _TO_; i++) { 
		string str  = string("Debug\\compiler s tests\\semantics\\") + IntToStr(i) + string(".in");
		string str1 = string("tests/semantics/") + IntToStr(i) + string(".out");

		const char* s = str.c_str();
		const char* s1 = str1.c_str();
		
		freopen("output.txt", "w", stdout);
		system(s);
		freopen("CON", "w", stdout);
		if( !test(i, CompareFiles("output.txt", s1)) )
			ALL_TESTS_SUCCESSFUL++;
	}
	*/
	///*
	int __FROM_ = 1;
	int __TO_   = 8;
	for(int i = __FROM_; i <= __TO_; i++) { 
		string str  = string("Debug\\compiler C tests\\generation\\") + IntToStr(i) + string(".in");
		string str1 = string("tests/generation/") + IntToStr(i) + string(".out");

		const char* s = str.c_str();
		const char* s1 = str1.c_str();
		
		freopen("log.txt", "w", stdout);
		system(s);
		freopen("output.txt", "w", stdout);
		system("out");
		freopen("CON", "w", stdout);
		if( !test(i, CompareFiles("output.txt", s1)) )
			ALL_TESTS_SUCCESSFUL++;
	}
	//*/
	if(ALL_TESTS_SUCCESSFUL == 0)
		cout << endl << "All test successful =)";
	else
		cout << endl << "Several test error =(";

	cout << endl << endl;

	system("PAUSE");
	fclose(stdout);
	return 0;
}
