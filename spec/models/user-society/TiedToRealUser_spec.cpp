#include <iostream>

#include "off_spec/off_spec.hpp"

#include "TiedToRealUser.hpp"

static const std::string TEST_NAME_RU = "Kevin Martin";

DESCRIBE_CLASS(TiedToRealUser)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;
	}

	IT(has_correct_starting_values)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o1;
		SHOULD_EQ(o1.get_real_user_name(), "");

		DescribedClass o2(TEST_NAME_RU);
		SHOULD_EQ(o2.get_real_user_name(), TEST_NAME_RU);
	}
}
