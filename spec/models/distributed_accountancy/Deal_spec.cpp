#include "Deal.hpp"


#include "off_spec/off_spec.hpp"

#include "Deal.hpp"

#ifdef USE_WT
#include "wt_spec_helpers.hpp"
#endif


DESCRIBE_CLASS(Deal)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;
	}
}
