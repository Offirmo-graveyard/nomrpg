#include "MoneyAmount.hpp"

#include <sstream>
#include <iomanip> // for set precision


MoneyAmount::MoneyAmount()
{
}
MoneyAmount::MoneyAmount(money_amount_internal_representation value, const Currency& currency)
{
	init(value, currency);
}
MoneyAmount::MoneyAmount(double value, const Currency& currency)
{
	init(dec::decimal_cast<MONEY_DECIMALS>(value), currency);
}
MoneyAmount::MoneyAmount(double value, std::string currency_code)
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

std::string MoneyAmount::get_human_representation() const
{
	// hat tip http://stackoverflow.com/a/900414/587407
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(2) << dec::decimal_cast<2>(m_internal_value);
	return ss.str();

}

void MoneyAmount::check_operands_compatibility(const MoneyAmount& a, const MoneyAmount& b)
{
	// check if currencies are the same
	if (a.get_currency() != b.get_currency())
	{
		throw("incompatible currencies");
	}
}

MoneyAmount MoneyAmount::operator+(const MoneyAmount& op)
{
	check_operands_compatibility(*this, op);

	MoneyAmount res(this->m_internal_value + op.m_internal_value, this->m_currency);

	return res;
}
MoneyAmount MoneyAmount::operator-(const MoneyAmount& op)
{
	check_operands_compatibility(*this, op);

	MoneyAmount res(this->m_internal_value - op.m_internal_value, this->m_currency);

	return res;
}
MoneyAmount MoneyAmount::operator*(const MoneyAmount& op)
{
	check_operands_compatibility(*this, op);

	MoneyAmount res(this->m_internal_value * op.m_internal_value, this->m_currency);

	return res;
}

MoneyAmount MoneyAmount::operator +=(const MoneyAmount& op)
{
	check_operands_compatibility(*this, op);

	this->m_internal_value += op.m_internal_value;

	return *this;
}
MoneyAmount MoneyAmount::operator -=(const MoneyAmount& op)
{
	check_operands_compatibility(*this, op);

	this->m_internal_value -= op.m_internal_value;

	return *this;
}


bool MoneyAmount::operator==(const MoneyAmount& op) const
{
	return (op.m_internal_value == this->m_internal_value && op.m_currency == this->m_currency);
}


std::ostream& operator<<(std::ostream& os, const MoneyAmount& obj)
{
	std::string helper;
	os << dec::toString<MONEY_DECIMALS>(obj.m_internal_value, helper);
	os << obj.m_currency.get_code();
	return os;
}
