#include "wt_spec_helpers.hpp"

#include "AppSessionInitializer.hpp"

#include "OffirmoWebApp.hpp"


namespace SpecHelp
{
	boost::shared_ptr< ::WtTestData > create_new_OWA_for_tests(enum ::WtEnvSetting env_mode)
	{
		// creates a data holder
		boost::shared_ptr<WtTestData> test_data(new WtTestData());

		// create a test environment
		test_data->env.reset(new Wt::Test::WTestEnvironment());

		// change env config
		switch(env_mode)
		{
		case WT_ENV_LANG_DE:
			test_data->env->setLocale("de");
			break;
		case WT_ENV_LANG_FR:
			test_data->env->setLocale("fr");
			break;
		case WT_ENV_DEFAULT:
			/* fallthrough */
		default:
			// nothing special
			break;
		}

		/*
		 * Setup a session, will typically be done once at application startup.
		 * If needed, we can setup several sessions pointing to different databases.
		 */
		boost::shared_ptr<WtDboData> auth_dbo_data(new WtDboData());
		auth_dbo_data->init_for_sqllite(":memory:");

		boost::shared_ptr<WtDboData> main_dbo_data(new WtDboData());
		main_dbo_data->init_for_sqllite(":memory:");
		main_dbo_data->ensure_db_schema();

		// the real, full fledged app
		test_data->app.reset(new OffirmoWebApp(*(test_data->env.get()), auth_dbo_data, main_dbo_data));

		return test_data;
	}

	boost::shared_ptr<AppSession> create_dbo_session(std::string info_str)
	{
		WtDboDataSchemaExtension empty; // declares no additional classes

		return create_dbo_session(empty, info_str);
	}
	boost::shared_ptr<AppSession> create_dbo_session(const WtDboDataSchemaExtension& ext, std::string info_str)
	{
		/*
		 * Setup a session, like at application startup.
		 */

		if (info_str != ":memory:")
		{
			WtDboData::delete_file_if_it_exists(info_str);
		}

		WtDboData dbo_data;
		dbo_data.init_for_sqllite(info_str);
		dbo_data.ensure_db_schema(ext);

		boost::shared_ptr<AppSession> session(new AppSession());
		session->register_main_dbo_session(dbo_data.cnx, dbo_data.session); // take reference on 'cnx' and 'session' shared ptrs

		// Note : its OK if dbo_data is freed, cnx and session are not since they are shared pointers

		return session;
	}

	void fill_session_up_to_ru(boost::shared_ptr<AppSession> app_session)
	{
		::Wt::Dbo::Session& session = *app_session->main_dbo_session();

		/////// RealUser
		// create it
		{
			::Wt::Dbo::Transaction transaction(session);
			RealUser* p = new RealUser(COMMON_TEST_NAME_U1);
			::Wt::Dbo::ptr<RealUser> ptr = session.add(p);
			transaction.commit();
			// register it into the default session object
			app_session->register_current_user(ptr);
		}
	}

	void fill_session_up_to_vw(boost::shared_ptr<AppSession> app_session)
	{
		fill_session_up_to_ru(app_session);

		// VW and co. can be set automatically
		AppSessionSetter setter(app_session);
		setter.attempt_set_up_to_vwgp();
	}

	void fill_session_up_to_vwi(boost::shared_ptr<AppSession> app_session)
	{
		fill_session_up_to_ru(app_session);

		// VW and co. can be set automatically
		AppSessionSetter setter(app_session);
		setter.attempt_set_up_to_vwip();
	}

	void fill_session_up_to_avatar(boost::shared_ptr<AppSession> app_session)
	{
		fill_session_up_to_ru(app_session);

		// VW and co. can be set automatically
		AppSessionSetter setter(app_session);
		setter.attempt_set_up_to_avatar();
/*
		::Wt::Dbo::Session& session = *app_session->main_dbo_session();

		/////// Avatar
		// create it
		{
			::Wt::Dbo::Transaction transaction(session);
			Avatar* p = new Avatar(*app_session->real_user(), *app_session->instance(), COMMON_TEST_NAME_VW1I1U1A1);
			::Wt::Dbo::ptr<Avatar> ptr = session.add(p);
			transaction.commit();
			// register it into the default session object
			app_session->register_current_avatar(ptr);
		}*/
	}
} // namespace SpecHelp
