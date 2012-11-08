#include "scanner.h"

int main() {
	for(int i = 1; i < 2; i++) {
		Scanner scanner("/tests/test_input" + i);
		test(i, CompareFiles("output", "test"));
	}

	system("PAUSE");
	return 0;
}