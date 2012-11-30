#include "PersistableObject.hpp"

PersistableObject::PersistableObject():
	Object(),
	m_creation_date(boost::posix_time::microsec_clock::universal_time())
{

}
