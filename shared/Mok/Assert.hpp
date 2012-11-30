/*
#include "Mok/Assert.hpp"
 */

#ifndef ASSERT_HPP_
#define ASSERT_HPP_

#include "Mok/Exception.hpp"

namespace Mok
{

class AssertionFailedException : public Exception
{
public:
	AssertionFailedException(const std::string& s = ""): Exception("Assertion failed : " + s + " !") {}
};

#define MOK_ASSERT(cond) do { if(!(cond)) { throw Mok::AssertionFailedException(#cond); } } while(0)

} /* namespace Mok */

#endif /* ASSERT_HPP_ */
