#include "TBRPGGame.hpp"

#include "Mok/Assert.hpp"


// for generating our click events.
// Boost is soooo perfect !
#include <boost/random/discrete_distribution.hpp>

#include "Avatar.hpp"
#include "TBRPGEventRealization.hpp"
#include "TBRPGEventRepo.hpp"



static const TBRPGEvent sc_first_event("first_event");


TBRPGGame::TBRPGGame():
		m_total_good_click_count(0),
		m_current_bad_click_count(0)
{
	common_inits();
}

TBRPGGame::TBRPGGame(const Avatar& avatar):
		TiedToAvatar(avatar),
		m_total_good_click_count(0),
		m_current_bad_click_count(0)
{
	common_inits();
}
void TBRPGGame::common_inits()
{
	m_STI_type = get_class_name();
	m_random_infos.current_min_display_count = 0;
}


void TBRPGGame::register_possible_event(const TBRPGEvent& evt)
{
	TBRPGameLocalEventId id = TBRPGGame::get_id_for(evt);

	m_events[id].event = evt;
	m_events[id].available = true;
	m_events[id].display_count = 0;

	m_random_infos.events_as_array.push_back(id);
	m_random_infos.events_probabilities.push_back(1); // by default
}

void TBRPGGame::register_possible_events(const TBRPGEventRepo& event_repo)
{
	// reset all
	m_events.clear();
	m_random_infos.events_as_array.clear();
	m_random_infos.events_probabilities.clear();

	// reinit with given events
	for(std::vector<TBRPGEvent>::const_iterator it = event_repo.events.begin(); it != event_repo.events.end(); ++it)
	{
		const TBRPGEvent& evt = *it;
		register_possible_event(evt);
	}
}


int TBRPGGame::get_actual_click_delay_s() const
{
	int delay = 10000000; // stupid value so far

	if (m_last_valid_click_datetime.is_not_a_date_time())
	{
		// no last valid click date : this is the first click, can click now without any delay
		delay = 0;
	}
	else
	{
		// Wait time is arbitrary (depending on lot of params)
		// It is stored in a variable. Just check it.

		// already elapsed time ?
		int elapsed_seconds = get_elapsed_seconds_since_last_correct_click();

		// compare it to wait time
		delay = (elapsed_seconds >= m_current_total_expected_wait_s) ? 0 : (m_current_total_expected_wait_s - elapsed_seconds);
	}

	return delay;
}


int TBRPGGame::get_elapsed_seconds_since_last_correct_click() const
{
	boost::posix_time::ptime current_datetime = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration diff = current_datetime - m_last_valid_click_datetime;
	int elapsed_seconds = diff.total_seconds();
	assert(elapsed_seconds >= 0);
	return elapsed_seconds;
}


int TBRPGGame::get_nominal_wait_delay_for_current_state_in_s() const
{
	// here we use a secret formula !
	return m_total_good_click_count*m_total_good_click_count;
}
int TBRPGGame::get_punishment_wait_delay_for_current_state_in_s() const
{
	// here we use a secret formula !
	return m_current_bad_click_count*10;

}


void TBRPGGame::click()
{
	// seen : usage of this object not fully initialized. Check :
	MOK_ASSERT(m_events.size());

	bool click_is_valid = false; // for now, until found otherwise

	click_is_valid = (get_actual_click_delay_s() == 0); // so far

	if (!click_is_valid)
		bad_click();
	else
		good_click();
}


void TBRPGGame::bad_click()
{
	// track it
	m_current_bad_click_count++;

	// reset countdown (sorry ! Your fault !)
	m_last_valid_click_datetime = boost::posix_time::microsec_clock::universal_time();

	//xxx + add punishment
	m_current_total_expected_wait_s =   get_elapsed_seconds_since_last_correct_click()   // current time waited is lost
	                                  + get_nominal_wait_delay_for_current_state_in_s()  // reset wait
	                                  + get_punishment_wait_delay_for_current_state_in_s(); // and add a punishment !
}
void TBRPGGame::good_click()
{
	if (m_total_good_click_count == 0)
	{
		// a special result for the first case only
		m_last_correct_click_event.realize(sc_first_event);
	}
	else
	{
		// pick up a random event
		TBRPGameLocalEventId id = pick_new_click_event();
		MOK_ASSERT(m_events[id].available);
		m_last_event_id = id;
		m_events[id].display_count++;
		m_last_correct_click_event.realize(m_events[id].event);
	}

	m_last_event_id = m_last_correct_click_event.get_id();
	m_total_good_click_count++;
	m_last_valid_click_datetime = boost::posix_time::microsec_clock::universal_time();
	m_current_total_expected_wait_s = get_nominal_wait_delay_for_current_state_in_s();
	// reset some vars tied to last click
	m_current_bad_click_count = 0;
}


void TBRPGGame::update_probability_infos()
{
	/// first of first, the random generator
	/*if(!m_random_infos.generator_is_seeded)
	{
		m_random_infos.raw_random_gen.seed(m_random_infos.seed);
		m_random_infos.generator_is_seeded = true;
	}*/

	/// first gather some infos
	{
		// first identify lowest display count
		bool init_done = false;
		for(std::map<TBRPGameLocalEventId, EventInfos>::iterator it = m_events.begin(); it != m_events.end(); ++it)
		{
			if(!init_done)
			{
				m_random_infos.current_min_display_count = it->second.display_count;
				init_done = true;
			}
			else if(it->second.display_count < m_random_infos.current_min_display_count)
			{
				m_random_infos.current_min_display_count = it->second.display_count;
			}
			if(m_random_infos.current_min_display_count == 0)
			{
				break; // no need to continue looping, will never be < 0
			}
		}
		std::cout << "min display count = " << m_random_infos.current_min_display_count << std::endl;
	}

	/// now fill the probabilities accordingly
	m_random_infos.events_probabilities.clear();

	for(std::vector<TBRPGameLocalEventId>::iterator it = m_random_infos.events_as_array.begin(); it != m_random_infos.events_as_array.end(); ++it)
	{
		double desired_probability = 1; // normal

		if(m_events[*it].event.get_id() == m_last_event_id) // note that we compare real ids, not local ids
		{
			// already displayed last time
			// we make it so that it can't be selected this time
			desired_probability = 0;
		}
/*		else if(m_events[*it].display_count > m_random_infos.current_min_display_count)
		{
			// already displayed more than others, no chance of being selected.
			desired_probability = 0;
		}*/
		else
		{
			// this event is one of the last often selected, it is a candidate to be selected
			// proba stays !0
		}

		std::cout << "event " << *it << " : displayed " << m_events[*it].display_count << " times -> proba : " << desired_probability << std::endl;
		m_random_infos.events_probabilities.push_back(desired_probability);
	}
}

TBRPGGame::TBRPGameLocalEventId TBRPGGame::pick_new_click_event()
{
	update_probability_infos();
	MOK_ASSERT(m_random_infos.events_probabilities.size());
	MOK_ASSERT(m_random_infos.events_as_array.size());

	// Note : see boost::random::discrete_distribution documentation
	// for an example of use

	//std::vector<double>::iterator itr = m_random_infos.events_probabilities.begin();

	boost::random::discrete_distribution<> dist(m_random_infos.events_probabilities.begin(), m_random_infos.events_probabilities.end());

	int selected_id = dist(m_random_infos.raw_random_gen);

	return m_random_infos.events_as_array[selected_id];
}


#ifdef USE_WT
#include "AppSessionUtils.hpp" // for specializing for BoringRPG

/// specialization for our class, see AppSessionUtils.hpp
template<>
Wt::Dbo::collection< Wt::Dbo::ptr<TBRPGGame> > find_virtual_world_elements_request(boost::shared_ptr<AppSession>& app_session, Wt::Dbo::Session& dbo_session)
{
	Wt::Dbo::Transaction transaction(dbo_session);
	Wt::Dbo::collection< Wt::Dbo::ptr<TBRPGGame> > elements = dbo_session.find<TBRPGGame>()
								.where("virtual_world = ?").bind(app_session->virtual_world()->get_denomination())
								.where("instance = ?").bind(app_session->instance()->get_denomination())
								.where("avatar = ?").bind(app_session->avatar()->get_denomination());
	transaction.commit();
	return elements;
}
#endif // USE_WT
