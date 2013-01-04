#include "Deal.hpp"

#include <boost/date_time/gregorian/gregorian.hpp>


Deal::Deal(const std::string& denomination):
	NamedPersistableObject(denomination),
	m_direction(UNKNOWN)
{

}

Deal::Deal(const DistributedAccount& da, const std::string& denomination):
	NamedPersistableObject(denomination),
	TiedToDistributedAccount(da),
	m_direction(UNKNOWN)
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

std::string Deal::get_direction_as_string() const
{
	std::string ret = "<error>"; // by default

	switch(m_direction)
	{
	case UNKNOWN:
		ret = "???";
		break;
	case INPUT:
		ret = "input";
		break;
	case OUTPUT:
		ret = "output";
		break;
	case INTERNAL:
		ret = "internal";
		break;
	default:
		throw Mok::UnknownEnumValueException("Deal::Direction");
		break;
	}

	return ret;
}

std::string Deal::get_direction_as_string_symbol() const
{
	std::string ret = "<error>"; // by default

	switch(m_direction)
	{
	case UNKNOWN:
		ret = "?";
		break;
	case INPUT:
		ret = "+";
		break;
	case OUTPUT:
		ret = "-";
		break;
	case INTERNAL:
		ret = " ";
		break;
	default:
		throw Mok::UnknownEnumValueException("Deal::Direction");
		break;
	}

	return ret;
}
