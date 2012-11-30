#include <iostream>
#include <typeinfo>

#include "off_spec/off_spec.hpp"
#include "off_spec/off_spec_wt_model.hpp"


#include "ArchetypeGroup.hpp"
// related objects
//#include "avatar.hpp"

//#include "wt_spec_helpers.hpp"

DESCRIBE_CLASS(ArchetypeGroup)
{
	IT(should_have_rspec_like_initializers)
	{
		TRACE_CURRENT_TEST();

		//SpecHelp::OffirmoWebApp::ensure_basic_user();
	}

	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;
	}

	IT(has_correct_STI_info)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		//std::cout << "  typeid = " << typeid(o).name() << ", class name = " << o.get_class_name() << " [" << o.get_STI_type() << "]" << std::endl;

		SHOULD_EQ(o.get_class_name(), "ArchetypeGroup");
		SHOULD_EQ(o.get_STI_type(),   "ArchetypeGroup");
	}

	IT(has_correct_starting_values)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		// check initial state
		//SHOULD_BE_TRUE(o.get_last_valid_click_datetime().isNull());
		//SHOULD_EQ(0, o.get_total_click_count());
		//SHOULD_EQ(0, o.get_bad_click_count());
		//SHOULD_EQ(0, o.get_click_delay_s());
	}
}
