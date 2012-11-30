/*
#include "off_spec.hpp"
 */

#ifndef OFF_SPEC_HPP_INCLUDED__
#define OFF_SPEC_HPP_INCLUDED__


#include <string>

// we are based on UnitTest++
#include "UnitTest++.h"

/// redefinitions, our style

// TestMacros
#define DESCRIBE(Name)  SUITE(Name)
/* start a suite with a "describe"
 * the param MUST be a class
 */
#define DESCRIBE_CLASS(ClassName)        \
	SUITE(ClassName)                      \
	{                                     \
		typedef ClassName DescribedClass;  \
	}                                     \
	namespace Suite##ClassName

#define IT(Name)         TEST(Name)

// CheckMacros
#define SHOULD(value)                        CHECK((value))
#define SHOULD_BE_TRUE(value)                SHOULD((value))
#define SHOULD_NOT(value)                    CHECK(!(value))
#define SHOULD_BE_FALSE(value)               SHOULD_NOT((value))

#define SHOULD_BE(property, object)          CHECK(object.is_##property())
#define SHOULD_NOT_BE(property, object)      CHECK(!object.is_##property())

#define SHOULD_BE_NULL(value)                SHOULD((value) == NULL)
#define SHOULD_NOT_BE_NULL(value)            SHOULD((value) != NULL)

#define SHOULD_BE_A(SuperClass, object)      CHECK(dynamic_cast<SuperClass *>(&(object)))
#define SHOULD_NOT_BE_A(SuperClass, object)  CHECK( (dynamic_cast<SuperClass *>(&(object))) == 0)

#define SHOULD_EQ(value1, value2)            CHECK((value1) == (value2))
#define SHOULD_NOT_EQ(value1, value2)        CHECK((value1) != (value2))

#define SHOULD_EQ_0(value)                   SHOULD_EQ(0, value)
#define SHOULD_NOT_EQ_0(value)               SHOULD_NOT_EQ(0, value)

#define SHOULD_BE_GREATER_THAN(value, value_greater)            CHECK((value_greater) > (value))
#define SHOULD_BE_GREATER_OR_EQUAL_THAN(value, value_greater)   CHECK((value_greater) >= (value))

// todo make better
#define SHOULD_HAVE_A_TEST_HERE              CHECK(0)

// REM : no () for those params
#define SHOULD_THROW(exception_type, code)   CHECK_THROW(code, exception_type)


template<typename TElem>
bool off_spec_is_empty(const TElem& elem);

#define SHOULD_BE_EMPTY(value)               CHECK(off_spec_is_empty(value))
#define SHOULD_NOT_BE_EMPTY(value)           CHECK(!off_spec_is_empty(value))


// "Don't Bother Me With Const"
/*template<typename T>
const T& off_spec_DBMWC(T& e) { return e; }

template<typename T>
const T& off_spec_DBMWC(const T& e) { return e; }
*/

/* a common way to compute "distance" between to elems
 * useful for "close to" expectations.
 */
template<typename TElem>
float off_spec_max_distance_considered_close(const TElem& useless_param_just_for_easy_templating);

template<typename TElem>
float off_spec_distance(const TElem& elem1, const TElem& elem2);
// specializations must be used for specific types


#define SHOULD_BE_WITHIN(dist, a, b)   CHECK(off_spec_distance((a), (b)) < dist)
#define SHOULD_BE_CLOSE(a, b)          SHOULD_BE_WITHIN(off_spec_max_distance_considered_close((a)), (a), (b))



// helper function
std::string prettify_test_name(char const* const testName);

#define TRACE_CURRENT_TEST()   \
	do { \
		std::cout << "- checking if " << m_details.suiteName << " " << prettify_test_name(m_details.testName) << "..." << std::endl; \
	} while(0)

#endif // OFF_SPEC_HPP_INCLUDED__
