
#include "off_spec.hpp"

std::string prettify_test_name(char const* const testName)
{
	return std::string(testName);
}

template<>
bool off_spec_is_empty(const std::string& value)
{
	return value.empty();
}
