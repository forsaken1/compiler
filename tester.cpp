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

void MakeFiles(string dir, int from, int to) {
	for(int i = from; i <= to; i++) {
		string str  = string("tests/" + dir + "/" + to_string((_Longlong)i) + ".in");
		string str1 = string("tests/" + dir + "/" + to_string((_Longlong)i) + ".out");
		const char* s = str.c_str();
		const char* s1 = str1.c_str();
		ofstream fo(s, ios::out);
		ofstream fo1(s1, ios::out);
		fo.close();
		fo1.close();
	}
}

int main() {
	int ERROR_TEST_COUNT = 0;
	/*
	int FROM = 1;
	int TO	 = 60; //60
	cout << endl << "Scanner tests" << endl << endl;
	for(int i = FROM; i <= TO; i++) { 
		string str  = string("Debug\\compiler L tests\\scanner\\" + to_string((_Longlong)i) + ".in");
		string str1 = string("tests/scanner/" + to_string((_Longlong)i) + ".out");

		freopen("output.txt", "w", stdout);
		system(str.c_str());
		freopen("CON", "w", stdout);
		if( !test(i, CompareFiles("output.txt", str1.c_str())) )
			ERROR_TEST_COUNT++;
	}
	//*/
	/*
	int _FROM = 1;
	int _TO   = 88; //88
	cout << endl << "Parser tests" << endl << endl;
	for(int i = _FROM; i <= _TO; i++) { 
		string str  = string("Debug\\compiler S tests\\parser\\" + to_string((_Longlong)i) + ".in");
		string str1 = string("tests/parser/" + to_string((_Longlong)i) + ".out");

		freopen("output.txt", "w", stdout);
		system(str.c_str());
		freopen("CON", "w", stdout);
		if( !test(i, CompareFiles("output.txt", str1.c_str())) )
			ERROR_TEST_COUNT++;
	}
	//*/
	/*
	int _FROM_ = 1;
	int _TO_   = 1;
	cout << endl << "Semantics tests" << endl << endl;
	for(int i = _FROM_; i <= _TO_; i++) { 
		string str  = string("Debug\\compiler s tests\\semantics\\" + to_string((_Longlong)i) + ".in");
		string str1 = string("tests/semantics/" + to_string((_Longlong)i) + ".out");

		freopen("output.txt", "w", stdout);
		system(str.c_str());
		freopen("CON", "w", stdout);
		if( !test(i, CompareFiles("output.txt", str1.c_str())) )
			ERROR_TEST_COUNT++;
	}
	//*/
	///*
	int __FROM_ = 44;
	int __TO_   = 45;
	cout << endl << "Generation tests" << endl << endl;
	for(int i = __FROM_; i <= __TO_; i++) { 
		string str  = string("Debug\\compiler C tests\\generation\\" + to_string((_Longlong)i) + ".in");
		string str1 = string("tests/generation/" + to_string((_Longlong)i) + ".out");

		freopen("log.txt", "w", stdout);
		system(str.c_str());
		freopen("output.txt", "w", stdout);
		system("out");
		freopen("CON", "w", stdout);
		if( !test(i, CompareFiles("output.txt", str1.c_str())) )
			ERROR_TEST_COUNT++;
	}
	//*/
	if(ERROR_TEST_COUNT == 0)
		cout << endl << "All test successful =)";
	else
		cout << endl << "Several test error =(";

	cout << endl << endl;

	system("PAUSE");
	fclose(stdout);
	return 0;
}



