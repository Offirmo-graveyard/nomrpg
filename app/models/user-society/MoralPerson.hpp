/*
#include "MoralPerson.hpp"
 */
#ifndef MORAL_PERSON_HPP_
#define MORAL_PERSON_HPP_

#include "Person.hpp"

class MoralPerson: public Person
{
public:
	MoralPerson(const std::string& p_simple_denomination = Person::cm_default_denomination):
		Person(p_simple_denomination) { }

	// then, it can have some properties
	virtual bool is_natural() { return false; }
	virtual bool is_moral()   { return true; }
};

#endif /* MORAL_PERSON_HPP_ */
