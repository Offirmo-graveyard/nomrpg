/* main, entry point of the Wt app.
 */

#include <exception> // for gcc verbose termination handler


#include <Wt/WServer>

#include "WtDboData.hpp"



// the we app we will launch
#include "OffirmoWebApp.hpp"


/* This method is written following Wt docs.
 */
static Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
	/*
	 * We can read information from the environment to decide whether
	 * the user has permission to start a new application.
	 * We could also check any prerequisite.
	 */
	bool proceed = /* ... */ true; // TODO if needed

	Wt::WApplication *app = NULL; // for now
	if (!proceed)
	{
		// an empty app just to display an error
		app = new Wt::WApplication(env);
		app->redirect("error.html");
		app->quit();
	}
	else
	{
		/*
		 * Setup a session, will typically be done once at application startup.
		 * If needed, we can setup several sessions pointing to different databases.
		 */
		boost::shared_ptr<WtDboData> auth_dbo_data(new WtDboData());
		auth_dbo_data->init_for_sqllite("database_auth.db");

		boost::shared_ptr<WtDboData> main_dbo_data(new WtDboData());
		main_dbo_data->init_for_sqllite(/*"database.db"*/);
		main_dbo_data->ensure_db_schema();

		// the real, full fledged app
		app = new OffirmoWebApp(env, auth_dbo_data, main_dbo_data);
	}
	
	return app;
}


/* The main function, launched at start
 */
int main(int argc, char *argv[])
{
	std::cout << "main([" << argc << " args] : ...)" << std::endl;

#ifdef __GNUC__
	// gcc hack, cf. http://gcc.gnu.org/onlinedocs/libstdc++/manual/termination.html#support.termination.verbose
	std::set_terminate(__gnu_cxx::__verbose_terminate_handler);
#endif

	try
	{
		Wt::WServer server(argv[0]);

		server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
		server.addEntryPoint(Wt::Application, createApplication);

		Session::configureAuth();

		if (server.start())
		{
			//std::string property_name = "application-settings.max-request-size";
			//std::string bundle_path;
			//Wt::WServer::instance()->readConfigurationProperty( property_name, bundle_path);
			//std::cout << "value for \"" << property_name << "\" is : " << bundle_path << std::endl;
			//return 1;

			Wt::WServer::waitForShutdown();
			server.stop();
		}
	} catch (Wt::WServer::Exception& e)
	{
		std::cerr << e.what() << std::endl;
	} catch (std::exception &e)
	{
		std::cerr << "exception: " << e.what() << std::endl;
	} catch (...)
	{
		std::cerr << "unknown exception" << std::endl;
	}

	return 0;
}
