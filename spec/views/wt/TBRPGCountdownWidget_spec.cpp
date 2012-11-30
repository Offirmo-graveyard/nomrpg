#include "off_spec/off_spec.hpp"

#include "TBRPGCountdownWidget.hpp"

#include "wt_spec_helpers.hpp"


static const std::string sc_default_radix = "TBRPG.default.";

DESCRIBE_CLASS(TBRPGCountdownWidget)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		boost::shared_ptr<WtTestData> test_data = SpecHelp::create_new_OWA_for_tests();

		DescribedClass o(NULL, sc_default_radix);
	}

	IT(is_builds)
	{
		TRACE_CURRENT_TEST();

		boost::shared_ptr<WtTestData> test_data = SpecHelp::create_new_OWA_for_tests();

		DescribedClass o(NULL, sc_default_radix);

		o.reset(30);
		std::cout << o.main_text() << std::endl;
		SHOULD_EQ(o.main_text(), "Encore 30 secondes avant de pouvoir cliquer...");

		o.reset(60);
		std::cout << o.main_text() << std::endl;
		SHOULD_EQ(o.main_text(), "Encore 1 minute avant de pouvoir cliquer...");

		o.reset(120);
		std::cout << o.main_text() << std::endl;
		SHOULD_EQ(o.main_text(), "Encore 2 minutes avant de pouvoir cliquer...");

		o.reset(183);
		std::cout << o.main_text() << std::endl;
		SHOULD_EQ(o.main_text(), "Encore 3 minutes et 3 secondes avant de pouvoir cliquer...");

		o.reset(3600);
		std::cout << o.main_text() << std::endl;
		SHOULD_EQ(o.main_text(), "Encore 1 heure avant de pouvoir cliquer...");

		o.reset(3601);
		std::cout << o.main_text() << std::endl;
		SHOULD_EQ(o.main_text(), "Encore 1 heure et 1 seconde avant de pouvoir cliquer...");

		o.reset(3661);
		std::cout << o.main_text() << std::endl;
		SHOULD_EQ(o.main_text(), "Encore 1 heure, 1 minute et 1 seconde avant de pouvoir cliquer...");

		o.reset(3600 * 24);
		std::cout << o.main_text() << std::endl;
		SHOULD_EQ(o.main_text(), "Encore 1 jour avant de pouvoir cliquer...");

		o.reset(3600 * (24+3));
		std::cout << o.main_text() << std::endl;
		SHOULD_EQ(o.main_text(), "Encore 1 jour et 3 heures avant de pouvoir cliquer...");

		o.reset(3600 * (24 + 3) + 20 * 60);
		std::cout << o.main_text() << std::endl;
		SHOULD_EQ(o.main_text(), "Encore 1 jour, 3 heures et 20 minutes avant de pouvoir cliquer...");

		o.reset(3600 * (24 + 3) + 20 * 60 + 59);
		std::cout << o.main_text() << std::endl;
		SHOULD_EQ(o.main_text(), "Encore 1 jour, 3 heures, 20 minutes et 59 secondes avant de pouvoir cliquer...");
	}
}
