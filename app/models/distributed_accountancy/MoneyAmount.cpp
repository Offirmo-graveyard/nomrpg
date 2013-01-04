#include "MoneyAmount.hpp"

#include <sstream>
#include <iomanip> // for set precision

#include "Mok/Exception.hpp"


const std::string MoneyAmount::cm_default_incorrect_currency_code = "?";

MoneyAmount::MoneyAmount():
		is_uninitialized(true)
{
	// useful ?
	init(dec::decimal_cast<MONEY_DECIMALS>(0), Currency(cm_default_incorrect_currency_code));
}
MoneyAmount::MoneyAmount(money_amount_internal_representation value, const Currency& currency):
		is_uninitialized(false)
{
	init(value, currency);
}
MoneyAmount::MoneyAmount(double value, const Currency& currency):
		is_uninitialized(false)
{
	init(dec::decimal_cast<MONEY_DECIMALS>(value), currency);
}
MoneyAmount::MoneyAmount(double value, std::string currency_code):
		is_uninitialized(false)
{
	Currency currency(currency_code);
	init(dec::decimal_cast<MONEY_DECIMALS>(value), currency);
}

void MoneyAmount::init(money_amount_internal_representation value, const Currency& currency)
{
	m_internal_value = value;
	m_currency = currency;
}



MoneyAmount::~MoneyAmount()
{
	// TODO Auto-generated destructor stub
}

void MoneyAmount::set_currency(const Currency& currency)
{
	if (is_uninitialized)
	{
		// OK
		is_uninitialized = false;
		m_currency = currency;
	}
	else if(currency == m_currency)
	{
		// same ccurrency, no change, OK
	}
	else
	{
		// forbidden to change currency as simply as that !
		// (may hide a need for conversion)
		throw Mok::DataNotRedefinableException("Can't change currency as simply as that !");
	}
}


std::string MoneyAmount::get_human_representation() const
{
	// hat tip http://stackoverflow.com/a/900414/587407
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(2) << dec::decimal_cast<2>(m_internal_value) << m_currency.get_code();
	return ss.str();
}
std::string MoneyAmount::get_human_representation_without_currency() const
{
	// hat tip http://stackoverflow.com/a/900414/587407
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(2) << dec::decimal_cast<2>(m_internal_value);
	return ss.str();
}

void MoneyAmount::check_operands_compatibility(const MoneyAmount& a, const MoneyAmount& b)
{
	// check if currencies are compatibles

	// uninitialized ~ no currency
	if (    a.is_uninitialized
	    and b.is_uninitialized)
	{
		// one uninitialized is OK but not two
		throw Mok::IncoherentDataException("Operation between unknown currencies !");
	}
	else if (a.get_currency() != b.get_currency()
	         and not a.is_uninitialized
	         and not b.is_uninitialized)
	{
		throw Mok::IncoherentDataException("incompatible currencies : " + a.get_currency().get_code() + "/" + b.get_currency().get_code());
	}
}

MoneyAmount MoneyAmount::operator+(const MoneyAmount& op)
{
	check_operands_compatibility(*this, op);

	MoneyAmount res(*this);

	// reuse existing method
	res += op;

	return res;
}
MoneyAmount MoneyAmount::operator-(const MoneyAmount& op)
{
	check_operands_compatibility(*this, op);

	MoneyAmount res(*this);

	// reuse existing method
	res -= op;

	return res;
}
/*
MoneyAmount MoneyAmount::operator*(const MoneyAmount& op)
{
	check_operands_compatibility(*this, op);

	MoneyAmount res(this->m_internal_value * op.m_internal_value, this->m_currency);

	return res;
}
*/
MoneyAmount MoneyAmount::operator +=(const MoneyAmount& op)
{
	check_operands_compatibility(*this, op);

	// works even if one is uninitialized
	this->m_internal_value += op.m_internal_value;

	if(is_uninitialized)
	{
		this->m_currency = op.m_currency;
		is_uninitialized = false;
	}

	return *this;
}
MoneyAmount MoneyAmount::operator -=(const MoneyAmount& op)
{
	check_operands_compatibility(*this, op);

	this->m_internal_value -= op.m_internal_value;

	if(is_uninitialized)
	{
		this->m_currency = op.m_currency;
		is_uninitialized = false;
	}

	return *this;
}


bool MoneyAmount::operator==(const MoneyAmount& op) const
{
	return (op.m_internal_value == this->m_internal_value && op.m_currency == this->m_currency);
}
bool MoneyAmount::operator!=(const MoneyAmount& op) const
{
	return (op.m_internal_value != this->m_internal_value || op.m_currency != this->m_currency);
}


std::ostream& operator<<(std::ostream& os, const MoneyAmount& obj)
{
	std::string helper;
	os << dec::toString<MONEY_DECIMALS>(obj.m_internal_value, helper);
	os << obj.m_currency.get_code();
	return os;
}
