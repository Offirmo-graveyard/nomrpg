/*
#include "AppSessionUtils.hpp"
 */

#ifndef APPSESSIONUTILS_HPP_
#define APPSESSIONUTILS_HPP_


// most of the Offirmo Web App things use the same model :
//  -> does it exists ?
//     -> Yes -> I take it
//     -> No -> I create it
// we use a set of template functions to share this behaviour.


#include <typeinfo>

#include "AppSession.hpp"

#include "Mok/Exception.hpp"


// generic function, to be specialized for each type
template<class T>
Wt::Dbo::collection< Wt::Dbo::ptr<T> > find_virtual_world_elements_request(boost::shared_ptr<AppSession>& app_session, Wt::Dbo::Session& dbo_session)
{
	// throws an error. Should be specialized.
	std::string error("no find_virtual_world_element_request() specialization for ");
	error += typeid(T).name();
	throw Mok::IncoherentDataException( error );
}


template<class T>
Wt::Dbo::ptr<T> find_unique_virtual_world_element_int(boost::shared_ptr<AppSession>& app_session, Wt::Dbo::Session& dbo_session)
{
	Wt::Dbo::ptr<T> element_ptr; // empty, evaluates to false

	// find available in database
	Wt::Dbo::collection< Wt::Dbo::ptr<T> > elements = find_virtual_world_elements_request<T>(app_session, dbo_session);
	std::cout << "We have " << elements.size() << " such virtual worlds elements." << std::endl;

	switch(elements.size())
	{
	case 0:
		// none yet
		// do nothing, return value stays empty
		break;
	case 1:
		// just one : return it
		{
			// an active transaction is needed
			Wt::Dbo::Transaction transaction(dbo_session);
			element_ptr = *(elements.begin());
			transaction.commit();
		}
		break;
	default:
		// more than one : error !
		{
			std::string error("found too many ");
			error += typeid(T).name();
			error += " in find_unique_virtual_world_element()";
			throw Mok::IncoherentDataException();
		}
		break;
	}

	return element_ptr;
}


template<class T>
Wt::Dbo::ptr<T> find_unique_virtual_world_element(boost::shared_ptr<AppSession> app_session)
{
	Wt::Dbo::Session& dbo_session = *(app_session->main_dbo_session());

	return find_unique_virtual_world_element_int<T>(app_session, dbo_session);
}



#endif /* APPSESSIONUTILS_HPP_ */
