#include <iostream>
#include <typeinfo>

#include "off_spec/off_spec.hpp"

#include "TiedToAvatar.hpp"


static const std::string TEST_NAME_VW  = "Very Own Testing Grounds";
static const std::string TEST_NAME_VWI = "Beta";
static const std::string TEST_NAME_AV  = "Kevin le roxxor";

DESCRIBE_CLASS(TiedToAvatar)
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
		SHOULD_EQ(o1.get_instance_name(), "");
		SHOULD_EQ(o1.get_avatar_name(), "");

		DescribedClass o2(TEST_NAME_VW, TEST_NAME_VWI, TEST_NAME_AV);
		SHOULD_EQ(o2.get_virtual_world_name(), TEST_NAME_VW);
		SHOULD_EQ(o2.get_instance_name(), TEST_NAME_VWI);
		SHOULD_EQ(o2.get_avatar_name(), TEST_NAME_AV);
	}
}
