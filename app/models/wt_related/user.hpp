/*
#include "user.hpp"
 */

#ifndef __USER_HPP_INCLUDED__
#define __USER_HPP_INCLUDED__

#include <string>

#include <Wt/Dbo/Types>
#include <Wt/WGlobal>

class User;
typedef Wt::Auth::Dbo::AuthInfo<User> AuthInfo;

class User
{
public:template<class Action>
	void persist(Action& a)
	{
	}
};

DBO_EXTERN_TEMPLATES(User)

#endif /* USER_HPP_ */
