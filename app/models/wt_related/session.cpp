#include "session.hpp"

#include "Wt/Auth/AuthService"
#include "Wt/Auth/HashFunction"
#include "Wt/Auth/PasswordService"
#include "Wt/Auth/PasswordStrengthValidator"
#include "Wt/Auth/PasswordVerifier"
#include "Wt/Auth/GoogleService"
#include "Wt/Auth/FacebookService"
#include "Wt/Auth/Dbo/AuthInfo"
#include "Wt/Auth/Dbo/UserDatabase"

using namespace Wt;

/* shared service classes
 */
namespace {

  class MyOAuth : public std::vector<const Auth::OAuthService *>
  {
  public:
    ~MyOAuth()
    {
      for (unsigned i = 0; i < size(); ++i)
	delete (*this)[i];
    }
  };

  Auth::AuthService myAuthService;
  Auth::PasswordService myPasswordService(myAuthService);
  MyOAuth myOAuthServices;

}


void Session::configureAuth()
{
	/* The Wt::Auth::AuthService
	 * is configured to support "remember-me" functionality, and email verification.
	 */
	myAuthService.setAuthTokensEnabled(true, "logincookie");
	myAuthService.setIdentityPolicy(Auth::EmailAddressIdentity); // we prefer login by email adress
	myAuthService.setEmailVerificationEnabled(true);

	/* The Wt::Auth::PasswordService
	 * needs a hash function to safely store passwords.
	 * You can actually define more than one hash function,
	 * which is useful only if you want to migrate to a new hash function
	 * while still supporting existing passwords.
	 * When a user logs in, and he is not using the "preferred"
	 * hash function, his password will be rehashed with the preferred one.
	 * In this example, we will use bcrypt which is included as a hash function in Wt::Auth.
	 */
	Auth::PasswordVerifier *verifier = new Auth::PasswordVerifier();
	verifier->addHashFunction(new Auth::BCryptHashFunction(7));
	// We also enable password attempt throttling: this mitigates brute force password guessing attempts.
	myPasswordService.setVerifier(verifier);
	myPasswordService.setAttemptThrottlingEnabled(true);
	//myPasswordService.setStrengthValidator(new Auth::PasswordStrengthValidator());

	/* Finally, we also use one (but later, perhaps more)
	 * Wt::Auth::OAuthService classes.
	 * You need one service per identity provider.
	 * Until OpenIDConnect is supported by most providers,
	 * each provider requires a proprietary method
	 * to access identity information using the authorized OAuth2.0 token.
	 * In this case, we only add Google as an identity provider.
	 */
	/*if (Wt::Auth::GoogleService::configured())
		myOAuthServices.push_back(new Wt::Auth::GoogleService(myAuthService));*/

	/*if (Wt::Auth::FacebookService::configured())
		myOAuthServices.push_back(new Wt::Auth::FacebookService(myAuthService));*/
}


Session::Session(const std::string& sqliteDb)
	: connection_(sqliteDb)
{
	connection_.setProperty("show-queries", "true");

	setConnection(connection_);

	mapClass<User>("user");
	mapClass<AuthInfo>("auth_info");
	mapClass<AuthInfo::AuthIdentityType>("auth_identity");
	mapClass<AuthInfo::AuthTokenType>("auth_token");

	try
	{
		createTables();
		std::cerr << "Created database." << std::endl;
	} catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "Using existing database";
	}

	users_ = new UserDatabase(*this);
}

Session::~Session()
{
  delete users_;
}


Auth::AbstractUserDatabase& Session::users() { return *users_; }

dbo::ptr<User> Session::user() const
{
  if (login_.loggedIn()) {
    dbo::ptr<AuthInfo> authInfo = users_->find(login_.user());
    return authInfo->user();
  } else
    return dbo::ptr<User>();
}

const Auth::AuthService& Session::auth()
{
	return myAuthService;
}

const Auth::PasswordService& Session::passwordAuth()
{
	return myPasswordService;
}

const std::vector<const Auth::OAuthService *>& Session::oAuth()
{
	return myOAuthServices;
}
