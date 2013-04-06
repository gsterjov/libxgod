#include "seatest/seatest.h"


void test_fixture_file();


void all_tests()
{
	test_fixture_file();
}


int main (int argc, char** argv)
{
	run_tests (all_tests);
	return 0;
}