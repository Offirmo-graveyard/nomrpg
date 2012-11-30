#include <iostream>
#include <typeinfo>

#include "off_spec/off_spec.hpp"

#include "TiedToVirtualWorld.hpp"

static const std::string TEST_NAME_VW = "Very Own Testing Grounds";

DESCRIBE_CLASS(TiedToVirtualWorld)
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
		SHOULD_EQ(o1.get_virtual_world_name(), "");

		DescribedClass o2(TEST_NAME_VW);
		SHOULD_EQ(o2.get_virtual_world_name(), TEST_NAME_VW);
	}
}
