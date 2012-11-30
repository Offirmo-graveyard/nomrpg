/* main, entry point.
 */

#include <exception> // for gcc verbose termination handler

// this main only launches unit tests
int check_specs(void);


int main(int argc, char *argv[])
{
#ifdef __GNUC__
	// gcc hack, cf. http://gcc.gnu.org/onlinedocs/libstdc++/manual/termination.html#support.termination.verbose
	std::set_terminate(__gnu_cxx::__verbose_terminate_handler);
#endif

	check_specs();
	
	return 0;
}
