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



// What is described here
// is already tested unitely in specific tests.
// But it is easier for the mind,
// to make a "big picture" test.
SUITE(DistributedAccountancyUseCases2)
{
	IT(allows_kalemya_case)
	{
		TRACE_CURRENT_TEST();

		MoneyAmount temp;

		// First a user : the owner / president / something
		RealUser ru_YEJ("Yves-Emmanuel Jutard");

		// first an account for a charity
		// we may give a default currency which may be used in UIs
		DistributedAccount da(ru_YEJ, "Kalemya", "EUR");

		// the account itself doesn't do computations,
		// it only records things.
		// Create a report to do computations :
		DistributedAccountReport dar(da);

		// the account is empty
		std::cout << dar.theoretical_total_available().get_human_representation() << std::endl;
		SHOULD_EQ(dar.theoretical_total_available().get_human_representation(), "0.00EUR");

		// create sub-accounts inside the main account
		// and register them
		DistributedSubAccount dsu_donation_box(da, "Boite a dons");
		DistributedSubAccount dsu_bank_account(da, "Compte HSBC");
		DistributedSubAccount dsu_paypal(da, "Paypal");

		// other users : the donators
		// create an account for those donators in the charity accountancy
		DistributedSubAccount dsu_misc_donators(da, "Donateurs Kalemya divers");
		RealUser ru_JLM("Jacqueline Le Ménager");
		DistributedSubAccount dsu_JLM(da, ru_JLM.get_name());
		RealUser ru_NJ("Noëlle Jutard");
		DistributedSubAccount dsu_NJ(da, ru_NJ.get_name());

		// also an account for the president, which is a donator, too ;)
		DistributedSubAccount dsu_YEJ(da, ru_YEJ.get_name());

		// now the donators gives some money
		Deal donation_20100928;
		donation_20100928.set_amount(MoneyAmount(55, "EUR"));
		donation_20100928.set_trade_value_settlement_dates_YYYYMMDD("20100928");
		donation_20100928.set_direction(Deal::INPUT);
		donation_20100928.set_source(dsu_misc_donators);
		donation_20100928.set_destination(dsu_donation_box);
		dar.register_deal(donation_20100928);

		// the charity now has 55€
		SHOULD_EQ(dar.theoretical_total_available(), MoneyAmount(55, "EUR"));

		// and they are in the donation box
		temp = dar.theoretical_balance_of_sub_account(dsu_donation_box);
		SHOULD_EQ(temp, MoneyAmount(55, "EUR"));
		// balance of misc donors is 0 since it's a donation
		temp = dar.theoretical_balance_of_sub_account(dsu_misc_donators);
		SHOULD_EQ(temp, MoneyAmount(0, "EUR"));
		// balance of unconcerned donors is also unchanged
		temp = dar.theoretical_balance_of_sub_account(dsu_YEJ);
		SHOULD_EQ(temp, MoneyAmount(0, "EUR"));

		// now let's do some internal operations
		Deal internal_20100928_1;
		internal_20100928_1.set_amount(MoneyAmount(50, "EUR"));
		internal_20100928_1.set_trade_value_settlement_dates_YYYYMMDD("20100928");
		internal_20100928_1.set_direction(Deal::INTERNAL);
		internal_20100928_1.set_source(dsu_donation_box);
		internal_20100928_1.set_destination(dsu_YEJ);
		dar.register_deal(internal_20100928_1);

		// the charity still has 55€
		SHOULD_EQ(dar.theoretical_total_available(), MoneyAmount(55, "EUR"));
		// but the donation box has only 5€
		temp = dar.theoretical_balance_of_sub_account(dsu_donation_box);
		SHOULD_EQ(temp, MoneyAmount(5, "EUR"));
		// and YEJ holds 50€
		temp = dar.theoretical_balance_of_sub_account(dsu_YEJ);
		SHOULD_EQ(temp, MoneyAmount(50, "EUR"));

		// a new agent
		RealUser ru_AM("Arnaud Mailhos");
		DistributedSubAccount dsu_AM(da, ru_AM.get_name());
//		dar.register_sub_account(dsu_AM);

		// now YEJ sends the money to AM
		Deal internal_20100928_2;
		internal_20100928_2.set_amount(MoneyAmount(50, "EUR"));
		internal_20100928_2.set_trade_value_settlement_dates_YYYYMMDD("20100928");
		internal_20100928_2.set_direction(Deal::INTERNAL);
		internal_20100928_2.set_source(dsu_YEJ);
		internal_20100928_2.set_destination(dsu_AM);
		dar.register_deal(internal_20100928_2);

		// the charity still has 55€
		SHOULD_EQ(dar.theoretical_total_available(), MoneyAmount(55, "EUR"));
		// and YEJ holds 0€
		temp = dar.theoretical_balance_of_sub_account(dsu_YEJ);
		SHOULD_EQ(temp, MoneyAmount(0, "EUR"));
		// and AM holds 50€
		temp = dar.theoretical_balance_of_sub_account(dsu_AM);
		SHOULD_EQ(temp, MoneyAmount(50, "EUR"));

		// a new agent
		RealUser ru_Farm_House("Farm House");
		DistributedSubAccount dsu_Farm_House(da, ru_Farm_House.get_name());
//		dar.register_sub_account(dsu_Farm_House);

		// now AM uses the money for the cause !
		Deal expenses_20100930;
		expenses_20100930.set_amount(MoneyAmount(50, "EUR"));
		expenses_20100930.set_trade_value_settlement_dates_YYYYMMDD("20100930");
		expenses_20100930.set_direction(Deal::OUTPUT);
		expenses_20100930.set_source(dsu_AM);
		expenses_20100930.set_destination(dsu_Farm_House);
		dar.register_deal(expenses_20100930);

		// the charity now has 5€
		SHOULD_EQ(dar.theoretical_total_available(), MoneyAmount(5, "EUR"));

		// balance of Farm House is 0 since it's a donation
		temp = dar.theoretical_balance_of_sub_account(dsu_Farm_House);
		SHOULD_EQ(temp, MoneyAmount(0, "EUR"));
		// balance of AM is 0
		temp = dar.theoretical_balance_of_sub_account(dsu_AM);
		SHOULD_EQ(temp, MoneyAmount(0, "EUR"));

		// new operations
		// october
		dar.register_deal(Deal().on_date("20101025").is_input().of(172, "EUR").from(dsu_misc_donators).to(dsu_donation_box));
		dar.register_deal(Deal().on_date("20101030").is_internal().of(50, "EUR").from(dsu_donation_box).to(dsu_YEJ));
		dar.register_deal(Deal().on_date("20101030").is_internal().of(50, "EUR").from(dsu_YEJ).to(dsu_AM));
		dar.register_deal(Deal().on_date("20101030").is_output().of(50, "EUR").from(dsu_AM).to(dsu_Farm_House));
		// november
		dar.register_deal(Deal().on_date("20101130").is_input().of(30, "EUR").from(dsu_misc_donators).to(dsu_donation_box));
		dar.register_deal(Deal().on_date("20101130").is_internal().of(51, "EUR").from(dsu_donation_box).to(dsu_YEJ));
		dar.register_deal(Deal().on_date("20101130").is_internal().of(51, "EUR").from(dsu_YEJ).to(dsu_AM));
		dar.register_deal(Deal().on_date("20101130").is_output().of(51, "EUR").from(dsu_AM).to(dsu_Farm_House));

		// let's check results
		SHOULD_EQ(dar.theoretical_total_available(), MoneyAmount(106, "EUR"));
		SHOULD_EQ(dar.theoretical_balance_of_sub_account(dsu_donation_box), MoneyAmount(106, "EUR"));
		SHOULD_EQ(dar.theoretical_balance_of_sub_account(dsu_YEJ), MoneyAmount(0, "EUR"));
		SHOULD_EQ(dar.theoretical_balance_of_sub_account(dsu_AM), MoneyAmount(0, "EUR"));

		RealUser ru_LS("Laura Saboie");
		DistributedSubAccount dsu_LS(da, ru_LS.get_name());
		dar.register_deal(Deal().on_date("20101201").is_input().of(50, "EUR").from(dsu_misc_donators).to(dsu_LS));
		dar.register_deal(Deal().on_date("20101215").is_internal().of(100, "EUR").from(dsu_donation_box).to(dsu_YEJ));
		// this one is in fact an advance, since it exceeds source capacity
		dar.register_deal(Deal().on_date("20101216").is_internal().of(150, "EUR").from(dsu_donation_box).to(dsu_YEJ));

		// let's check results
		SHOULD_EQ(dar.theoretical_total_available(), MoneyAmount(156, "EUR"));
		SHOULD_EQ(dar.theoretical_balance_of_sub_account(dsu_donation_box), MoneyAmount(-144, "EUR"));
		SHOULD_EQ(dar.theoretical_balance_of_sub_account(dsu_YEJ), MoneyAmount(250, "EUR"));
		SHOULD_EQ(dar.theoretical_balance_of_sub_account(dsu_AM), MoneyAmount(0, "EUR"));
		SHOULD_EQ(dar.theoretical_balance_of_sub_account(dsu_LS), MoneyAmount(50, "EUR"));

		dar.register_deal(Deal().on_date("20101223").is_input().of(273, "EUR").from(dsu_misc_donators).to(dsu_donation_box));
		dar.register_deal(Deal().on_date("20101223").is_internal().of(51, "EUR").from(dsu_donation_box).to(dsu_YEJ));
		dar.register_deal(Deal().on_date("20101223").is_internal().of(51, "EUR").from(dsu_YEJ).to(dsu_AM));
		dar.register_deal(Deal().on_date("20101223").is_output().of(51, "EUR").from(dsu_AM).to(dsu_Farm_House));

		// new people
		DistributedSubAccount dsu_V(da, "Vincent");
		DistributedSubAccount dsu_G(da, "Gauthier");

		dar.register_deal(Deal().on_date("20101229").is_input().of(438.39, "EUR").from(dsu_misc_donators).to(dsu_V));
		dar.register_deal(Deal().on_date("20101229").is_input().of(200, "EUR").from(dsu_misc_donators).to(dsu_G));
		// Arnaud gave money in advance
		dar.register_deal(Deal().on_date("20110103").is_output().of(698, "EUR").from(dsu_AM).to(dsu_Farm_House));

		// let's check results
		SHOULD_EQ(dar.theoretical_balance_of_sub_account(dsu_donation_box), MoneyAmount(78, "EUR"));
		SHOULD_EQ(dar.theoretical_balance_of_sub_account(dsu_YEJ), MoneyAmount(250, "EUR"));
		SHOULD_EQ(dar.theoretical_balance_of_sub_account(dsu_AM), MoneyAmount(-698, "EUR"));
		SHOULD_EQ(dar.theoretical_balance_of_sub_account(dsu_LS), MoneyAmount(50, "EUR"));
		SHOULD_EQ(dar.theoretical_balance_of_sub_account(dsu_V), MoneyAmount(438.39, "EUR"));
		SHOULD_EQ(dar.theoretical_balance_of_sub_account(dsu_G), MoneyAmount(200, "EUR"));
		// (lost track of money available to the charity. That's what this app is made for ^^)

		// now let's do some more interesting stuff
		// let's compute how much someone gave
		SHOULD_EQ(dar.theoretical_input_from_sub_account(dsu_YEJ), MoneyAmount(0, "EUR"));

		dar.print_recap();

		// and how much someone benefited
		MoneyAmount res = dar.theoretical_output_to_sub_account(dsu_Farm_House);
		std::cout << "res = " << res.get_human_representation() << std::endl;
		SHOULD_EQ(res, MoneyAmount(1218.39, "EUR"));

	}
}

