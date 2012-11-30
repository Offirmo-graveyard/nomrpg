#include "off_spec/off_spec.hpp"

#include <Wt/WMessageResourceBundle>

#include "OffirmoWebApp.hpp"

#include "wt_spec_helpers.hpp"




DESCRIBE_CLASS(OffirmoWebApp)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		boost::shared_ptr<WtTestData> test_data = SpecHelp::create_new_OWA_for_tests();
		SHOULD_NOT_BE_NULL(test_data->env);
		SHOULD_NOT_BE_NULL(test_data->app);
	}

	IT(starts_in_correct_state)
	{
		TRACE_CURRENT_TEST();

		// check if it is in the correct state at creation
	}

	IT(has_all_needed_templates_rsrcs)
	{
		TRACE_CURRENT_TEST();

		// TODO
	}

	bool check_i18n_rsrc_control_strings_for_file( std::string ext_less_filename)
	{
		Wt::WMessageResourceBundle& mrb = Wt::WApplication::instance()->messageResourceBundle();
		std::string result_begin, result_end;
		bool ok_begin, ok_end;
		ok_begin = mrb.resolveKey(ext_less_filename + ".xml.begin", result_begin);
		ok_begin = ok_begin && (result_begin == "ok");
		ok_end = mrb.resolveKey(ext_less_filename + ".xml.end", result_end);
		ok_end = ok_end && (result_end == "ok");
		return ok_begin && ok_end;
	}

	bool check_i18n_rsrc_control_strings_for_localized_file( std::string ext_less_filename, std::string locale = "")
	{
		bool ok1 = check_i18n_rsrc_control_strings_for_file(ext_less_filename);
		bool ok2 = locale.empty() || check_i18n_rsrc_control_strings_for_file(ext_less_filename + "_" + locale);
		return ok1 && ok2;
	}

	bool check_all_i18n_rsrc_control_strings( std::string locale = "" )
	{
		bool ok1 = check_i18n_rsrc_control_strings_for_localized_file( "TBRPGStrings",   locale );
		bool ok2 = check_i18n_rsrc_control_strings_for_localized_file( "TBRPGTemplates", locale );
		bool ok3 = check_i18n_rsrc_control_strings_for_localized_file( "AppStrings",     locale );
		bool ok4 = check_i18n_rsrc_control_strings_for_localized_file( "AppTemplates",   locale );
		bool ok5 = check_i18n_rsrc_control_strings_for_localized_file( "AuthOverride",   locale );
		bool ok6 = check_i18n_rsrc_control_strings_for_localized_file( "AuthOriginal",   locale );
		return ok1 && ok2 && ok3 && ok4 && ok5 && ok6;
	}

	IT(has_all_needed_string_rsrcs)
	{
		TRACE_CURRENT_TEST();

		boost::shared_ptr<WtTestData> test_data = SpecHelp::create_new_OWA_for_tests();

		Wt::WApplication* app = Wt::WApplication::instance();
		SHOULD_NOT_BE_NULL(app);

		std::cout << "Current locale :" << app->locale() << std::endl;

		// try to convert a few resources
		//SHOULD_BE_TRUE(check_all_i18n_rsrc_control_strings());

		/*
		Wt::WMessageResourceBundle& mrb = app->messageResourceBundle();

		std::cout << "Default keys :" << std::endl;
		Wt::WFlags<Wt::WMessageResourceBundle::Scope> scope = Wt::WMessageResourceBundle::Default;
		const std::set<std::string> mrb_keys = mrb.keys(scope);
		for(std::set<std::string>::iterator it = mrb_keys.begin(); it != mrb_keys.end(); ++it)
		{
			std::string result;
			bool ok = mrb.resolveKey(*it, result);
			std::cout << *it << " = " << result << " (" << ok << ")" << std::endl;
		}

		std::cout << "Local keys :" << std::endl;
		scope = Wt::WMessageResourceBundle::Local;
		const std::set<std::string> mrb_keys_loc = mrb.keys(scope);

		for(std::set<std::string>::iterator it = mrb_keys_loc.begin(); it != mrb_keys_loc.end(); ++it)
		{
			std::string result;
			bool ok = mrb.resolveKey(*it, result);
			std::cout << *it << " = " << result << " (" << ok << ")" << std::endl;
		}*/

		Wt::WLocalizedStrings* lz = app->localizedStrings();
		SHOULD_NOT_BE_NULL(lz);
	}

	IT(has_a_working_i18n)
	{
		TRACE_CURRENT_TEST();

		boost::shared_ptr<WtTestData> test_data = SpecHelp::create_new_OWA_for_tests(WT_ENV_LANG_FR);

		Wt::WApplication* app = Wt::WApplication::instance();
		SHOULD_NOT_BE_NULL(app);

		std::cout << "Current locale :" << app->locale() << std::endl;

		// try to convert a few resources
		SHOULD_BE_TRUE(check_all_i18n_rsrc_control_strings());

		Wt::WMessageResourceBundle& mrb = app->messageResourceBundle();

		std::cout << "Default keys :" << std::endl;
		Wt::WFlags<Wt::WMessageResourceBundle::Scope> scope = Wt::WMessageResourceBundle::Default;
		const std::set<std::string> mrb_keys = mrb.keys(scope);
		for(std::set<std::string>::iterator it = mrb_keys.begin(); it != mrb_keys.end(); ++it)
		{
			std::string result;
			bool ok = mrb.resolveKey(*it, result);
			std::cout << *it << " = " << result << " (" << ok << ")" << std::endl;
		}

		std::cout << "Local keys :" << std::endl;
		scope = Wt::WMessageResourceBundle::Local;
		const std::set<std::string> mrb_keys_loc = mrb.keys(scope);

		for(std::set<std::string>::iterator it = mrb_keys_loc.begin(); it != mrb_keys_loc.end(); ++it)
		{
			std::string result;
			bool ok = mrb.resolveKey(*it, result);
			std::cout << *it << " = " << result << " (" << ok << ")" << std::endl;
		}

		Wt::WLocalizedStrings* lz = app->localizedStrings();
		SHOULD_NOT_BE_NULL(lz);

		// try to convert some resources
		Wt::WString test= Wt::WString::trn("TBRPG.default.countdown.seconds", 30).arg(30);
		std::cout << test << std::endl;
		SHOULD_EQ("30 secondes", test);
		test= Wt::WString::trn("TBRPG.default.countdown.seconds", 1).arg(1);
		std::cout << test << std::endl;
		SHOULD_EQ("1 seconde", test);
		test= Wt::WString::trn("TBRPG.default.countdown.seconds", 0).arg(0);
		std::cout << test << std::endl;
		SHOULD_EQ("0 secondes", test);
	}

}
