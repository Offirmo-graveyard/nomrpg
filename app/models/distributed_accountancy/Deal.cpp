#include "Deal.hpp"

#include <boost/date_time/gregorian/gregorian.hpp>


Deal::Deal(const std::string& denomination):
	NamedPersistableObject(denomination)
//	m_source_sub_account_id(DistributedSubAccount::cm_main_subaccount_name),
//	m_destination_sub_account_id(DistributedSubAccount::cm_main_subaccount_name)
{

}

Deal::Deal(const DistributedAccount& da, const std::string& denomination):
	NamedPersistableObject(denomination),
	TiedToDistributedAccount(da)
//	m_source_sub_account_id(DistributedSubAccount::cm_main_subaccount_name),
//	m_destination_sub_account_id(DistributedSubAccount::cm_main_subaccount_name)
{

}

Deal::~Deal()
{
}

void Deal::set_trade_value_settlement_times(boost::posix_time::ptime date_time)
{
	m_trade_date = m_value_date = m_settlement_date = date_time;
}

void Deal::set_trade_value_settlement_dates_YYYYMMDD(std::string date)
{
	boost::posix_time::ptime date_time(boost::gregorian::from_undelimited_string(date), boost::posix_time::time_duration(0,0,0));
	set_trade_value_settlement_times(date_time);
}
