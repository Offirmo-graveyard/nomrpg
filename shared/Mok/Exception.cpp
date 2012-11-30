#include "Exception.hpp"

namespace Mok
{

Exception::Exception(const std::exception& except) throw():
		std::exception(except)
{
}

Exception::Exception(const std::string& what_str) throw():
		m_what(what_str)
{ }

Exception::Exception(const std::string& what_str, const std::exception& wrapped) throw():
		m_what(what_str + "\nCaused by exception: " + wrapped.what())
{ }

Exception::~Exception() throw()
{ }


const char *Exception::what() const throw()
{
  return m_what.c_str();
}


} /* namespace Mok */
