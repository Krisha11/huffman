#include "autotest.h"

int Test::failedNum = 0;
int Test::totalNum = 0; 
bool Test::is_ok_now = true;

void Test::check(bool expr, const char  *filename, std::size_t lineNum) {
	if (!expr) {
		printf("Fail!!: %s %d\n", filename, (int)lineNum);
		is_ok_now = false;			
	}
}

void Test::start_test() {
	is_ok_now = true;	
}

void Test::end_test() {
	if (is_ok_now) {
		printf("Test passed\n");
	}
	else {
		printf("Test failed\n");
		Test::failedNum++;
	}
	Test::totalNum++;
}


void Test::showFinalResult() {
	printf("Failed: %d/%d\n", Test::failedNum, Test::totalNum);
	if (Test::failedNum == 0)
		printf("OK\n");
}

