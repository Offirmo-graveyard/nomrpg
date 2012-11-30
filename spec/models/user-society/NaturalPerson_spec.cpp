#include <iostream>

#include "off_spec/off_spec.hpp"

#include "NaturalPerson.hpp"


DESCRIBE_CLASS(NaturalPerson)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;
	}

	IT(responds_correctly_to_moral_or_natural)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		SHOULD_BE(natural, o);
		SHOULD_NOT_BE(moral, o);
	}

	IT(has_proper_inheritance)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		SHOULD_BE_A(Person, o);
	}
}
