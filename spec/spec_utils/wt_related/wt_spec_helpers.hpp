/*
#include "wt_spec_helpers.hpp"
 */

#ifndef WT_SPEC_HELPERS_HPP_
#define WT_SPEC_HELPERS_HPP_


#include <boost/shared_ptr.hpp>

#include <Wt/Test/WTestEnvironment>

#include "WtDboData.hpp"
#include "AppSession.hpp"

class OffirmoWebApp;


#define COMMON_TEST_NAME_U1 "Kevin"
#define COMMON_TEST_NAME_U2 "Leeroy Jenkins"

//#define TEST_NAME_VW1 = "Global Testing Grounds"
#define   COMMON_TEST_NAME_VW1I1 "Beta"
#define     COMMON_TEST_NAME_VW1I1U1A1 "Kevin le roxxor"
//#define   TEST_NAME_VW1I2 = "Gamma"
//#define TEST_NAME_VW2 = "Global Testing Grounds II"
//#define   TEST_NAME_VW2I1 = "Beta"
//#define   TEST_NAME_VW2I2 = "Zeta"




enum WtEnvSetting
{
	WT_ENV_DEFAULT,
	WT_ENV_LANG_DE,
	WT_ENV_LANG_FR
};
struct WtTestData
{
	boost::shared_ptr<Wt::Test::WTestEnvironment> env;
	boost::shared_ptr<OffirmoWebApp> app;
};

namespace SpecHelp
{
	boost::shared_ptr< ::WtTestData > create_new_OWA_for_tests(enum ::WtEnvSetting env_mode = WT_ENV_DEFAULT);

	boost::shared_ptr<AppSession> create_dbo_session(std::string info_str = ":memory:");
	boost::shared_ptr<AppSession> create_dbo_session(const WtDboDataSchemaExtension& ext, std::string info_str = ":memory:");

	// various fill functions
	// in order, the later includes the former
	void fill_session_up_to_ru(boost::shared_ptr<AppSession> app_session);
	void fill_session_up_to_vw(boost::shared_ptr<AppSession> app_session);
	void fill_session_up_to_vwi(boost::shared_ptr<AppSession> app_session);
	void fill_session_up_to_avatar(boost::shared_ptr<AppSession> app_session);
}

#endif /* WT_SPEC_HELPERS_HPP_ */
