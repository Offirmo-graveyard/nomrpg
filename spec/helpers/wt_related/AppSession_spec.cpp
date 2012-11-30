#include <iostream>
//#include <typeinfo>

#include "off_spec/off_spec.hpp"

#include "AppSession.hpp"

#include "wt_spec_helpers.hpp"


DESCRIBE_CLASS(AppSession)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;
	}
}
