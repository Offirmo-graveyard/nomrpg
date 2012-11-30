#include <iostream>
#include <typeinfo>

#include "off_spec/off_spec.hpp"

#ifdef USE_WT
#include "off_spec/off_spec_wt_model.hpp"
#include "wt_spec_helpers.hpp"
#include "AppSessionUtils.hpp"
#endif

#include "TBRPGGame.hpp"
#include "TBRPGEventRealization.hpp"
#include "TBRPGEventRepo.hpp"


// related objects
#include "Avatar.hpp"



/* Hacked version
 * with features for test.
 */
class TBRPGGameForTest : public TBRPGGame
{
public:
	TBRPGGameForTest() : TBRPGGame() {}
	TBRPGGameForTest(const Avatar& avatar) : TBRPGGame(avatar) {}

	// a test method to simulate time flow for test
	void simulate_time_flow_of_x_seconds(int seconds)
	{
		// hack : replace "last click time" with an older version
		m_last_valid_click_datetime = m_last_valid_click_datetime - boost::posix_time::seconds(seconds);
	}

	void simulate_time_flow_until_next_click()
	{
		simulate_time_flow_of_x_seconds(get_actual_click_delay_s());
	}

	// to be able to reproduce some tests
	void seed_random_generator(uint32_t seed)
	{
		m_random_infos.raw_random_gen.seed(seed);
	}

	std::string get_last_id() const { return m_last_event_id; }
};


// hack local save
std::string g_last_id;
int g_current_delay_s = 0;


// test events
static const std::string sc_event1_id = "event1";
static const TBRPGEvent sc_event1(sc_event1_id);

static const std::string sc_event2_id = "event2";
static const TBRPGEvent sc_event2(sc_event2_id);

static const std::string sc_event3_id = "event3";
static const TBRPGEvent sc_event3(sc_event3_id);

static TBRPGEventRepo s_event_repo;

static void init_event_repo_if_needed()
{
	static bool init_done = false;
	if (!init_done)
	{
		s_event_repo.register_event(sc_event1);
		s_event_repo.register_event(sc_event2);
		s_event_repo.register_event(sc_event3);
		init_done = true;
	}
}

DESCRIBE_CLASS(TBRPGGame)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;
	}

	IT(has_correct_STI_info)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		std::cout << "  typeid = " << typeid(o).name() << ", class name = " << o.get_class_name() << std::endl;

		SHOULD_EQ(o.get_class_name(), "TBRPGGame");
		SHOULD_EQ(o.get_STI_type(),   "TBRPGGame");
	}

	IT(has_correct_starting_values)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		// check initial state
		SHOULD_BE_TRUE(o.get_last_valid_click_datetime().is_not_a_date_time());
		SHOULD_EQ(0, o.get_total_good_click_count());
		SHOULD_EQ(0, o.get_current_bad_click_count());
		SHOULD_EQ(0, o.get_actual_click_delay_s());
	}

	IT(responds_correctly_to_a_correct_click)
	{
		TRACE_CURRENT_TEST();
		init_event_repo_if_needed();

		DescribedClass o;
		o.register_possible_events(s_event_repo);

		// check preliminary state
		SHOULD_EQ(0, o.get_actual_click_delay_s());
		SHOULD_EQ(0, o.get_total_good_click_count());
		SHOULD_EQ(0, o.get_current_bad_click_count());

		// click
		o.click();
		SHOULD_BE_TRUE(o.was_last_click_OK());

		// check result and modified state
		TBRPGEventRealization event = o.get_last_correct_click_event();
		SHOULD_BE_FALSE(event.get_id().empty());
		SHOULD_NOT_EQ(0, o.get_actual_click_delay_s());
		SHOULD_EQ(1, o.get_total_good_click_count());
		SHOULD_EQ(0, o.get_current_bad_click_count());
	}

	IT(responds_correctly_to_an_incorrect_click)
	{
		TRACE_CURRENT_TEST();
		init_event_repo_if_needed();

		TBRPGGameForTest o; // XXX we use a hacked version with debug features
		// (I'm confident that the tests are still relevant)
		o.register_possible_events(s_event_repo);

		o.click();
		// there should be a click delay
		SHOULD_NOT_EQ(0, o.get_actual_click_delay_s());

		{
			TBRPGEventRealization event = o.get_last_correct_click_event();
			SHOULD_BE_FALSE(event.get_id().empty());
		}

		// save the delay
		int delay_backup = o.get_actual_click_delay_s();

		// BAD click (too soon)
		// delay cannot be already expired under normal conditions
		o.click();

		// click should be NOK
		SHOULD_BE_FALSE(o.was_last_click_OK());

		// click counts should be correct
		SHOULD_EQ(1, o.get_total_good_click_count());
		SHOULD_EQ(1, o.get_current_bad_click_count());

		// and delay should have increased, as a punishment
		SHOULD_BE_GREATER_THAN(delay_backup, o.get_actual_click_delay_s());

		// save it and bad click again
		delay_backup = o.get_actual_click_delay_s();

		// BAD click again, delay cannot be expired (under normal conditions)
		o.click();

		// click should be NOK
		SHOULD_BE_FALSE(o.was_last_click_OK());

		// click counts should be correct
		SHOULD_EQ(1, o.get_total_good_click_count());
		SHOULD_EQ(2, o.get_current_bad_click_count());

		// and delay should have increased, as a punishment
		SHOULD_BE_GREATER_THAN(delay_backup, o.get_actual_click_delay_s());

		// now see if a correct click cleans the state
		// simulate time flow
		o.simulate_time_flow_of_x_seconds(o.get_actual_click_delay_s()+3);

		// and click (should be correct)
		int delay = o.get_actual_click_delay_s();
		SHOULD_EQ(0, delay);
		o.click();

		// click should be OK
		SHOULD_BE_TRUE(o.was_last_click_OK());
		{
			TBRPGEventRealization event = o.get_last_correct_click_event();
			SHOULD_BE_FALSE(event.get_id().empty());
		}

		// click counts should be correct
		SHOULD_EQ(2, o.get_total_good_click_count());
		SHOULD_EQ(0, o.get_current_bad_click_count()); // reseted
	}

	IT(never_answers_the_same_thing_in_a_row)
	{
		TRACE_CURRENT_TEST();
		init_event_repo_if_needed();

		TBRPGGameForTest o; // XXX we use a hacked version with debug features
		// (I'm confident that the tests are still relevant)
		o.register_possible_events(s_event_repo);

		o.click(); // first click is always ok, pass it.

		TBRPGEventRealization last_event(o.get_last_correct_click_event());

		for(int i=0; i<50; i++) // note : some bad random appeared only above 30
		{
			std::cout << "last event : " << last_event.get_id() << std::endl;

			// simulate adequate time flow
			o.simulate_time_flow_until_next_click();

			// click
			o.click();

			// click should be OK
			SHOULD_BE_TRUE(o.was_last_click_OK());

			TBRPGEventRealization event(o.get_last_correct_click_event());
			SHOULD_BE_FALSE(last_event.get_id() == event.get_id());

			// prepare next loop
			last_event = event;
		}
	}

	IT(increases_delay_every_good_click)
	{
		TRACE_CURRENT_TEST();
		init_event_repo_if_needed();

		TBRPGGameForTest o; // XXX we use a hacked version with debug features
		// (I'm confident that the tests are still relevant)
		o.register_possible_events(s_event_repo);

		unsigned int last_delay(0);

		for(int i=0; i<30; i++)
		{
			// simulate adequate time flow
			o.simulate_time_flow_until_next_click();

			// click
			o.click();

			// click should be OK
			SHOULD_BE_TRUE(o.was_last_click_OK());

			unsigned int new_delay = o.get_actual_click_delay_s();

			SHOULD_BE_TRUE(new_delay > last_delay);

			// prepare next loop
			last_delay = new_delay;
		}
	}

	IT(increases_delay_every_bad_click)
	{
		TRACE_CURRENT_TEST();
		init_event_repo_if_needed();

		TBRPGGame o;
		o.register_possible_events(s_event_repo);

		o.click(); // first click is always ok, pass it.

		unsigned int last_delay(o.get_actual_click_delay_s());

		for(int i=0; i<30; i++)
		{
			// click (no time flow -> bad)
			o.click();

			// click should be NOK
			SHOULD_BE_FALSE(o.was_last_click_OK());

			unsigned int new_delay = o.get_actual_click_delay_s();

			SHOULD_BE_TRUE(new_delay > last_delay);

			// prepare next loop
			last_delay = new_delay;
		}
	}

#ifdef USE_WT
	static boost::shared_ptr<AppSession> persistence_test_session;

	IT(has_a_Wt_session)
	{
		TRACE_CURRENT_TEST();

		persistence_test_session = SpecHelp::create_dbo_session();
		SpecHelp::fill_session_up_to_avatar(persistence_test_session);
	}

	TBRPGGameForTest* create_game_for_persistence_test()
	{
		TBRPGGameForTest* brpg = new TBRPGGameForTest(*(persistence_test_session->avatar()));
		brpg->register_possible_events(s_event_repo);

		// now let's "play" a little with our game
		brpg->click();
		brpg->simulate_time_flow_until_next_click();
		brpg->simulate_time_flow_of_x_seconds(2);
		brpg->click();
		brpg->simulate_time_flow_until_next_click();
		brpg->simulate_time_flow_of_x_seconds(20);
		brpg->click();
		brpg->simulate_time_flow_of_x_seconds(5);
		// oups
		brpg->click();
		brpg->simulate_time_flow_until_next_click();
		brpg->simulate_time_flow_of_x_seconds(2);
		brpg->click(); // OK
		brpg->simulate_time_flow_of_x_seconds(10);
		brpg->click(); // oups
		brpg->simulate_time_flow_of_x_seconds(1);
		brpg->click(); // reoups

		return brpg;
	}


	IT(is_persistable)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::Session& session = *(persistence_test_session->main_dbo_session());

		boost::posix_time::ptime current_test_start_datetime = boost::posix_time::microsec_clock::universal_time();

		TBRPGGameForTest* brpg = create_game_for_persistence_test();
		// ensure the counts
		SHOULD_BE_FALSE(brpg->was_last_click_OK());
		SHOULD_EQ(2, brpg->get_current_bad_click_count());
		SHOULD_EQ(4, brpg->get_total_good_click_count());
		// privately save some values
		g_last_id = brpg->get_last_id();
		g_current_delay_s = brpg->get_actual_click_delay_s();

		// now let's save it
		{
			TBRPGGame* brpg2 = dynamic_cast<TBRPGGame *>(brpg); // Safe
			Wt::Dbo::Transaction transaction(session);
			session.add(brpg2);
			transaction.commit();
			brpg2 = brpg = NULL; // can't use it anymore
		}
	}

	IT(is_depersistable)
	{
		TRACE_CURRENT_TEST();

		Wt::WDateTime current_test_start_datetime = Wt::WDateTime::currentDateTime();

		// we use the specially crafted RPGGame finder function
		Wt::Dbo::ptr<TBRPGGame> brpg_depersisted = find_unique_virtual_world_element<TBRPGGame>(persistence_test_session);

		// check the values we know and the values we stored earlier
		SHOULD_BE_FALSE(brpg_depersisted->was_last_click_OK());
		SHOULD_EQ(2, brpg_depersisted->get_current_bad_click_count());
		SHOULD_EQ(4, brpg_depersisted->get_total_good_click_count());
		// privately save some values
		SHOULD_EQ(g_current_delay_s, brpg_depersisted->get_actual_click_delay_s());

		// now monstruously hack it into a "for test" version
		TBRPGGameForTest brpg_dp2;
		brpg_dp2.TBRPGGame::operator=(*brpg_depersisted); // forcefully assign child class part
		brpg_dp2.register_possible_events(s_event_repo);
		// check additional value (thanks to test version)
		SHOULD_EQ(g_last_id, brpg_dp2.get_last_id());

		// we also create a friend game
		TBRPGGameForTest* brpg_similar = create_game_for_persistence_test();

		// seed them with the same value
		brpg_dp2.seed_random_generator(123);
		brpg_similar->seed_random_generator(123);

		// check equality of state
		SHOULD_EQ(brpg_dp2.was_last_click_OK(),           brpg_similar->was_last_click_OK());
		SHOULD_EQ(brpg_dp2.get_current_bad_click_count(), brpg_similar->get_current_bad_click_count());
		SHOULD_EQ(brpg_dp2.get_total_good_click_count(),  brpg_similar->get_total_good_click_count());
		SHOULD_EQ(brpg_dp2.get_last_id(),                 brpg_similar->get_last_id()); // same pseudo random -> same id

		// now lets simultaneously play with both at the same time
		// and ensure the same things happen
		for(int i=0; i<50; i++) // note : some bad random appeared only above 30
		{
			// simulate adequate time flow
			brpg_dp2.simulate_time_flow_until_next_click();
			brpg_similar->simulate_time_flow_until_next_click();

			// click
			brpg_dp2.click();
			brpg_similar->click();

			// click should be OK
			SHOULD_BE_TRUE(brpg_dp2.was_last_click_OK());

			// check equality of responses
			SHOULD_EQ(brpg_dp2.was_last_click_OK(),           brpg_similar->was_last_click_OK());
			SHOULD_EQ(brpg_dp2.get_actual_click_delay_s(),    brpg_similar->get_actual_click_delay_s()); // we execute so fast that it should always be ok
			SHOULD_EQ(brpg_dp2.get_current_bad_click_count(), brpg_similar->get_current_bad_click_count());
			SHOULD_EQ(brpg_dp2.get_total_good_click_count(),  brpg_similar->get_total_good_click_count());
			SHOULD_EQ(brpg_dp2.get_last_id(),                 brpg_similar->get_last_id()); // same pseudo random -> same id
		}
	}

	IT(should_be_unique_for_a_given_avatar)
	{
		TRACE_CURRENT_TEST();

		// an avatar can have only one game

		// TODO
	}

	IT(no_longer_has_a_Wt_session)
	{
		TRACE_CURRENT_TEST();

		persistence_test_session.reset();
	}
#endif // USE_WT
}
