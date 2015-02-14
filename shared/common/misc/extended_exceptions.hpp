#ifndef EXTENDED_EXCEPTIONS_HPP_
#define EXTENDED_EXCEPTIONS_HPP_
/*
#include "extended_exceptions.hpp"
 */



#include <string>
#include <exception>
#include <stdexcept>


namespace utils
{

/** An extended exception class.
 *  Inspired from Mok::Exception and Wt:WException
 */
class Exception : public std::exception
{
public:
    Exception() throw();
    explicit Exception(const std::string& what_str) throw();
    Exception(const std::exception&) throw();
    Exception(const std::string& what_str, const std::exception& wrapped) throw();

    virtual ~Exception() throw();

    virtual const char* what() const throw();

protected:
    std::string m_what;
};



// the very useful "not implemented"
class NotImplementedException : public Exception
{
public:
    NotImplementedException(const std::string& s = "") : Exception("This feature is not implemented yet : " + s + " !") {}
};

// this usually means that a new enum value has been added
// but corresponding code don't handle it yet
class UnknownEnumValueException : public Exception
{
public:
    UnknownEnumValueException(const std::string& s = "") : Exception("We encountered an unexpected enum value : " + s + " !") {}
};

} /* namespace Mok */

#endif /* EXTENDED_EXCEPTIONS_HPP_ */
