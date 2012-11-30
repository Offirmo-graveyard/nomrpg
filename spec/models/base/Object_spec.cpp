#include <iostream>
//#include <typeinfo>

#include "off_spec/off_spec.hpp"

#include "Object.hpp"



/* The Object class is pure virtual.
 * We need to derive from it.
 */
class ObjectForTest : public Object
{
public:
	virtual const std::string get_class_name() const { return "ObjectForTest"; }
};


DESCRIBE_CLASS(Object)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		ObjectForTest o;
	}

	IT(has_correct_STI_info)
	{
		TRACE_CURRENT_TEST();

		ObjectForTest o;

		//std::cout << "  typeid = " << typeid(o).name() << ", class name = " << o.get_class_name() << std::endl;

		SHOULD_EQ(o.get_class_name(), "ObjectForTest");
		//SHOULD_EQ(o.get_STI_type(),   "Avatar");
	}
}
