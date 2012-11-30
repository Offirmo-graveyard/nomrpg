#include <iostream>

#include "off_spec/off_spec.hpp"

#include "Person.hpp"


/* Since Person is an abstract class,
 * we must inherit it to test it.
 */
class TestablePerson : public Person
{
public:
	TestablePerson(const std::string& p_simple_denomination = Person::cm_default_denomination):
		Person(p_simple_denomination) { }
	virtual ~TestablePerson() {}

	/* We implement just what is needed.
	 * No need to test those methods. (except for coverage)
	 */
	virtual bool is_natural() { return false; }
	virtual bool is_moral()   { return false; }
};


DESCRIBE_CLASS(TestablePerson)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		// for coverage, we don't care about the result
		o.is_natural();
		o.is_moral();
	}

	IT(has_a_denomination)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o("Toto");

		SHOULD_EQ(o.get_denomination(), "Toto");
		SHOULD_NOT_EQ(o.get_denomination(), "Totoro");
	}

	IT(has_a_default_denomination)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		SHOULD_EQ(o.get_denomination(), Person::cm_default_denomination);
		SHOULD_EQ(o.get_denomination(), "Anonymous");
	}
}
