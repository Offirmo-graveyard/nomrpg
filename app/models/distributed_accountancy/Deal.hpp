/*
#include "Deal.hpp"
 */

#ifndef DEAL_HPP_
#define DEAL_HPP_


// ancestor classes
#include "NamedPersistableObject.hpp"
#include "TiedToDistributedAccount.hpp"

//#include <boost/date_time/gregorian/gregorian_types.hpp>

#include "MoneyAmount.hpp"
#include "DistributedSubAccount.hpp"


class Deal: public NamedPersistableObject, public TiedToDistributedAccount
{
public:
	enum Direction
	{
		INPUT,
		OUTPUT,
		INTERNAL
	};

	Deal(const std::string& denomination = "");
	Deal(const DistributedAccount& da, const std::string& denomination = "");

	virtual ~Deal();

	//////////////////////////////////// DBO ////////////////////////////////////
	// for STI (Single Type Inheritance). It MUST be equals to the class name.
	virtual const std::string get_class_name() const { return "Deal"; }
	//////////////////////////////////// DBO ////////////////////////////////////

	void set_direction(enum Direction direction) { m_direction = direction; }
	void set_amount(MoneyAmount amount) { m_amount = amount; }
	//void set_source(std::string sub_account_id)         { m_source_sub_account_id = sub_account_id; }
	void set_source(const DistributedSubAccount& dsa)   { m_source_sub_account = dsa; }
	//void set_destination(std::string sub_account_id)       { m_destination_sub_account_id = sub_account_id; }
	void set_destination(const DistributedSubAccount& dsa) { m_destination_sub_account = dsa; }

	void set_trade_value_settlement_times(boost::posix_time::ptime date_time);
	void set_trade_value_settlement_dates_YYYYMMDD(std::string date);

	enum Direction get_direction() const { return m_direction; }
	Currency get_currency() const { return m_amount.get_currency(); }
	MoneyAmount get_amount() const { return m_amount; }
	const DistributedSubAccount& get_source() const { return m_source_sub_account; }
	const DistributedSubAccount& get_destination() const { return m_destination_sub_account; }

protected:
	enum Direction m_direction;
	MoneyAmount m_amount;
	//std::string m_source_sub_account_id;
	//std::string m_destination_sub_account_id;
	DistributedSubAccount m_source_sub_account;
	DistributedSubAccount m_destination_sub_account;

	// TODO rework
	// http://en.wikipedia.org/wiki/Trade_date
	boost::posix_time::ptime m_trade_date; // date on which this deal was decided/agreed upon
	boost::posix_time::ptime m_value_date; // date on which this deal must be executed (usually = trade date)
	boost::posix_time::ptime m_settlement_date; // date on which this deal came into effect (money transferred)
};

#endif /* DEAL_HPP_ */
