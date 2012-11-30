/*
#include "Object.hpp"
 *
 * Note : I KNOW that having all class deriving from
 *        a single root clas is questionned.
 *        I personnaly find it useful.
 */

#ifndef OBJECT_HPP_INCLUDED__
#define OBJECT_HPP_INCLUDED__


#include <string>

/* This is a base virtual class
 */
class Object
{
public:
	Object();
	virtual ~Object(); // defining it just to make it virtual

	// for misc utils, it MUST be equals to the class name.
	virtual const std::string get_class_name() const = 0;
};

#endif // OBJECT_HPP_INCLUDED__
