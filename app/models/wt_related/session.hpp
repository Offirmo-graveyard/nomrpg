/*
#include "session.hpp"
 */
#ifndef __SESSION_HPP_INCLUDED__
#define __SESSION_HPP_INCLUDED__


#include "Wt/Auth/Login"

#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/backend/Sqlite3>


#include "Wt/Auth/AuthService"
#include "Wt/Auth/PasswordService"


#include "user.hpp"

namespace dbo = Wt::Dbo;

typedef Wt::Auth::Dbo::UserDatabase<AuthInfo> UserDatabase;

class Session : public dbo::Session
{
public:
	static void configureAuth();

	Session(const std::string& sqliteDb);
	virtual ~Session();

	dbo::ptr<User> user() const;

	/* A user may have several "identities" (facebook, google...)
	 * with corresponding credentials.
	 * This method returns the identities database for the user.
	 */
	Wt::Auth::AbstractUserDatabase& users();

	// get the login object
	Wt::Auth::Login& login() { return login_; }



	// the 3 utility classes.
	static const Wt::Auth::AuthService& auth();
	static const Wt::Auth::PasswordService& passwordAuth();
	static const std::vector<const Wt::Auth::OAuthService *>& oAuth();

private:
	dbo::backend::Sqlite3 connection_;
	UserDatabase *users_; // the credential database. In Wt, a user may have several credentials (google login, facebook login, etc.)
	Wt::Auth::Login login_; // current login information, that the login/logout widget will use

};

#endif // __SESSION_HPP_INCLUDED__
