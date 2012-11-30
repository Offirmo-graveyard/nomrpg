/*
#include "TBRPGEventRealization.hpp"
 */

#ifndef TBRPGEVENTREALIZATION_HPP_
#define TBRPGEVENTREALIZATION_HPP_


#include "TBRPGEvent.hpp"


class TBRPGEventRealization
{
public:
	TBRPGEventRealization();
	TBRPGEventRealization(TBRPGEvent event);
	virtual ~TBRPGEventRealization();

	std::string get_id() const { return m_event.get_id(); }

	void realize(const TBRPGEvent& evt);
protected:
	TBRPGEvent m_event;
};

#endif /* TBRPGEVENTREALIZATION_HPP_ */
