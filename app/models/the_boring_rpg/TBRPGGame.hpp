/*
#include "TBRPGGame.hpp"
 */

#ifndef THE_BORING_RPG_GAME_HPP_
#define THE_BORING_RPG_GAME_HPP_


// ancestor classes
#include "TiedToAvatar.hpp"
#include "PersistableObject.hpp"

#include <map>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/random/mersenne_twister.hpp> // random generator

class TBRPGEventRepo;
#include "TBRPGEventRealization.hpp"


/**
 *
 */
class TBRPGGame : public TiedToAvatar, public PersistableObject
{
public:
	TBRPGGame();
	TBRPGGame(const Avatar& avatar);
	void common_inits();

	// STI
	virtual const std::string get_class_name() const { return "TBRPGGame"; }

	// return the current delay before the next click is allowed (in s)
	// REM : this function return a different value depending on actual date
	virtual int get_actual_click_delay_s() const;

	// return the punishment for the last bad click
	// returns 0 if last click was not bad
	virtual int get_actual_punishment_delay_s() const { return get_punishment_wait_delay_for_current_state_in_s(); };

	// return the number of valid clicks so far (since the beginning of the game)
	virtual int get_total_good_click_count() const { return m_total_good_click_count; }

	// return the date of the last valid click
	virtual const boost::posix_time::ptime get_last_valid_click_datetime() const { return m_last_valid_click_datetime; }

	// return the count of bad clicks since last correct clicks
	// (resetted when a valid click occurs)
	virtual int get_current_bad_click_count() const { return m_current_bad_click_count; }

	// check if the last click was OK
	virtual bool was_last_click_OK() const { return m_current_bad_click_count == 0; }

	// click !
	void click();

	virtual TBRPGEventRealization get_last_correct_click_event() const { return m_last_correct_click_event; }

	void register_possible_events(const TBRPGEventRepo& events);

	////////////////////// DBO /////////////////////
#ifdef USE_WT
	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, m_last_valid_click_datetime,       "last_valid_click_date");
		Wt::Dbo::field(a, m_total_good_click_count,          "total_good_click_count");
		Wt::Dbo::field(a, m_current_bad_click_count,         "current_bad_click_count");
		Wt::Dbo::field(a, m_last_event_id,                   "last_event_id");
		Wt::Dbo::field(a, m_current_total_expected_wait_s,   "current_total_expected_wait_s");

		TiedToAvatar::persist_virtual(a);
		PersistableObject::persist_virtual(a);
	}
#endif // USE_WT
	////////////////////// DBO /////////////////////

protected:
	boost::posix_time::ptime m_last_valid_click_datetime;
	int m_total_good_click_count; // signed int because Wt::dbo doesn't handle unsigned ints
	int m_current_bad_click_count; // signed int because Wt::dbo doesn't handle unsigned ints
	std::string m_last_event_id; // useful to ensure that we never get twice the same event in a row

	TBRPGEventRealization m_last_correct_click_event; // XXX this field is not persisted (no need), so it default back to empty when unpersisted

	int m_current_total_expected_wait_s; // total expected wait from last correct click date
	                                     // can NOT be recomputed in case of a bad click,
	                                     // because depend on the actual date of the bad click
	                                     // signed int because Wt::dbo doesn't handle unsigned ints

	// sub-functions for computing wait time
	int get_elapsed_seconds_since_last_correct_click() const;
	int get_nominal_wait_delay_for_current_state_in_s() const;
	int get_punishment_wait_delay_for_current_state_in_s() const;

	// sub-functions of clic()
	// @see click
	void bad_click();
	void good_click();

	void register_possible_event(const TBRPGEvent& evt);

	// we need an event id
	// define it with a typedef with a method to compute it
	// so we have room for future optimizations
	typedef std::string TBRPGameLocalEventId;
	TBRPGameLocalEventId get_id_for(const TBRPGEvent& evt) const { return evt.get_id(); }

	struct EventInfos
	{
		TBRPGEvent event;
		unsigned int display_count; // how many times this event has been displayed
		bool available; // is this event available for selection ? (useful for events temporarily disabled)
	};
	std::map<TBRPGameLocalEventId, EventInfos> m_events;

	TBRPGameLocalEventId pick_new_click_event();

	// for random generation
	// one way of generating
	struct
	{
		boost::random::mt19937 raw_random_gen;
		//uint32_t seed; // will be used to seed the generator if not yet done
		//bool generator_is_seeded; // if false, then prior to a generation, the generator must be seeded with the seed aboce
		// Those vars are updated every good click.
		std::vector<TBRPGameLocalEventId> events_as_array; // iterable 0..n array referencing the events
		std::vector<double> events_probabilities; // adapted probability of this event being selected on next click
		unsigned int current_min_display_count;
	} m_random_infos;
	void update_probability_infos();

};


#endif /* THE_BORING_RPG_GAME_HPP_ */
