#include <iostream>
#include <typeinfo>

#include "off_spec/off_spec.hpp"

#ifdef USE_WT
#include "off_spec/off_spec_wt_model.hpp"
#include "wt_spec_helpers.hpp"
#endif

#include "RealUser.hpp"
#include "DistributedAccount.hpp"
#include "DistributedSubAccount.hpp"
#include "Deal.hpp"
#include "DistributedAccountReport.hpp"


static const std::string TEST_NAME_ASSOC1 = "Pitié";

static const std::string TEST_NAME_OWNER = "Frédéric Ozanam";

static const std::string TEST_NAME_U1 = "Sully Prud'homme";
//static const std::string TEST_NAME_SUBACC_U1 = "Pitié";


// What is described here
// is already tested unitely in specific tests.
// But it is easier for the mind,
// to make a "big picture" test.
SUITE(DistributedAccountancyUseCases)
{
	IT(allows_use_case_1)
	{
		TRACE_CURRENT_TEST();

		// First a user : the owner / president / something
		RealUser ruo(TEST_NAME_OWNER);

		// first an account for a charity
		// we must give a currency
		DistributedAccount da1(ruo, TEST_NAME_ASSOC1, "EUR");
		// the account itself doesn't do computations,
		// it only records things.
		// Create a report to do computations :
		DistributedAccountReport da1r(da1);

		// the account is empty
		SHOULD_EQ(da1r.theoretical_total_available().get_human_representation(), "0.00");

		// another user : the donator
		RealUser ru1(TEST_NAME_U1);

		// create an account for this donator in the charity accountancy
		DistributedSubAccount dsu1(da1, ru1.get_name());
		da1r.register_sub_account(dsu1);

		// now the donator gives some money
		MoneyAmount a1(100, "EUR");
		Deal d1;
		d1.set_amount(a1);
		d1.set_direction(Deal::INPUT);
		d1.set_source(dsu1);
		//d1.set_destination(da1.get_main_sub_account_name());
		da1r.register_deal(d1);


		// the charity now has 100€
		SHOULD_EQ(da1r.theoretical_total_available(), MoneyAmount(100, "EUR"));
	}


	IT(allows_use_case_2)
	{
		TRACE_CURRENT_TEST();

		// real life kalemya use case

		// First a user : the president or treasurer
		const std::string KAL_BOSS = "YEJ";
		RealUser ru_kal_boss(KAL_BOSS);

		// account for our charity
		// we must give a currency
		const std::string KAL_NAME = "Kalemya";
		DistributedAccount da(ru_kal_boss, KAL_NAME, "EUR");
		// the account itself doesn't do computations,
		// it only records things.
		// Create a report to do computations :
		DistributedAccountReport dar(da);

		// the account is empty
		SHOULD_EQ(dar.theoretical_total_available().get_human_representation(), "0.00");

		// create a sub-account inside the main account
		const std::string DONATION_BOX_NAME = "Donation box";
		DistributedSubAccount dsu_donation_box(da, DONATION_BOX_NAME);
		// register it
		dar.register_sub_account(dsu_donation_box);

		// now there are donations in the donation box
		// from anonymous users
		Deal donation_20100928;
		donation_20100928.set_amount(MoneyAmount(55, "EUR"));
		donation_20100928.set_trade_value_settlement_dates_YYYYMMDD("20100928");
		donation_20100928.set_direction(Deal::INPUT);
		//donation_20100928.set_source(dsu1);
		donation_20100928.set_destination(dsu_donation_box);
		dar.register_deal(donation_20100928);

		// the charity now has 55€
		SHOULD_EQ(dar.theoretical_total_available(), MoneyAmount(55, "EUR"));

		// and they are in the donation box
		SHOULD_EQ(dar.theoretical_balance_of_sub_account(dsu_donation_box), MoneyAmount(55, "EUR"));
	}
}

