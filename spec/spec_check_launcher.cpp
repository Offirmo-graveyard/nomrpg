#include <stdlib.h>
#include <stdio.h>

#include <UnitTest++.h>


int check_specs(void)
{
	unsigned int failed_tests_count = 0;

	failed_tests_count = UnitTest::RunAllTests();

	// now display a very clear result check
	// because it's not always easy to assert the total result.
	printf("\n\n");
	if (failed_tests_count)
	{
		printf(""
"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
"!!!  XXX SOME TESTS FAILED ! XXX  !!!\n"
"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	}
	else
	{
		printf(""
"+------------------------------------------------+\n"
"| Tests were apparently successful (this time ;) |\n"
"+------------------------------------------------+\n");
	}
	printf("\n\n");

	return (failed_tests_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

