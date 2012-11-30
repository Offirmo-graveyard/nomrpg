/*
#include "NaturalPerson.hpp"
 */
#ifndef NATURAL_PERSON_HPP_
#define NATURAL_PERSON_HPP_

#include "Person.hpp"

class NaturalPerson: public Person
{
public:
	NaturalPerson(const std::string& p_simple_denomination = Person::cm_default_denomination):
		Person(p_simple_denomination) { }

	// then, it can have some properties
	virtual bool is_natural() { return true; }
	virtual bool is_moral()   { return false; }
};

#endif /* NATURAL_PERSON_HPP_ */
