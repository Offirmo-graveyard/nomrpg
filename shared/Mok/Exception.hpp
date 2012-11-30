/*
#include "Mok/Exception.hpp"
 */
#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_


#include <string>
#include <exception>

#include <stdexcept>


namespace Mok
{

/** An extended exception class.
 *  Inspired from Wt:WException
 */
class Exception : public std::exception
{
public:
	explicit Exception(const std::string& what_str) throw();
	Exception(const std::exception&) throw();
	Exception(const std::string& what_str, const std::exception& wrapped) throw();

	virtual ~Exception() throw();

	//Exception& operator= (const exception&) throw();

	virtual const char* what() const throw();

protected:
  std::string m_what;
};


// now let's define a bunch of common exceptions :
class IncoherentDataException : public Exception
{
public:
	IncoherentDataException(const std::string& s = ""): Exception("Unexpected incoherent data : " + s + " !") {}
};
class DataNotAvailableException : public Exception
{
public:
	DataNotAvailableException(const std::string& s = "") : Exception("This data is not available : " + s + " !") {}
};
class DataNotRedefinableException : public Exception
{
public:
	DataNotRedefinableException(const std::string& s = "") : Exception("This data cannot be redefined : " + s + " !") {}
};
class MissingRequiredDataException : public Exception
{
public:
	MissingRequiredDataException(const std::string& s = "") : Exception("Critical data are missing : " + s + " !") {}
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

#endif /* EXCEPTION_HPP_ */
