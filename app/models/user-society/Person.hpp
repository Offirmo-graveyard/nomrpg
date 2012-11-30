#ifndef PERSON_HPP_
#define PERSON_HPP_


#include "NamedPersistableObject.hpp"

/* This is a base virtual class.
 * It holds a "person", a generic object
 * designing someone.
 */
class Person : public NamedPersistableObject
{
public:
	static const std::string cm_default_denomination;

	Person(const std::string& p_denomination = Person::cm_default_denomination):
		NamedPersistableObject(p_denomination)
		{ m_STI_type = get_class_name(); }
	virtual ~Person() {} // declaring it just for the sake of being virtual

	//////////////////////////////////// DBO ////////////////////////////////////
	// for STI (Single Type Inheritance). It MUST be equals to the class name.
	virtual const std::string get_class_name() const { return "Person"; }

#ifdef USE_WT
	// this method is to be called inside the persist() method
	// of the immediate child.
	template<class Action>
	void persist_virtual(Action& a)
	{
		//dbo::field(a, m_simple_denomination, "denomination");
		//dbo::id(a, m_simple_denomination, "denomination", Person::MAX_DENOMINATION_SIZE);

		NamedPersistableObject::persist_virtual(a);
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

	// then, it can have some properties
	virtual bool is_natural() = 0;
	virtual bool is_moral() = 0;

protected:
	//std::string m_simple_denomination; -> inherited
};


#endif /* PERSON_HPP_ */
