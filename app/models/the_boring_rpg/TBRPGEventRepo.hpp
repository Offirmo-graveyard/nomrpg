/*
#include "TBRPGEventRepo.hpp"
 */

#ifndef TBRPGEVENTREPO_HPP_
#define TBRPGEVENTREPO_HPP_


#include <vector>

#include "TBRPGEvent.hpp"


/* A class destined to "store" a set of BRPG events
 * It is used to be shared amongst several thread without having
 * to reload events from database all the time.
 */
class TBRPGEventRepo
{
public:
	void register_event(const TBRPGEvent& evt) { events.push_back(evt); }

	// no need to hide anything, this is just an utility class without any actual role
	std::vector<TBRPGEvent> events;
};

#endif /* TBRPGEVENTREPO_HPP_ */
