/*
#include "MoneyAmount.hpp"
 */
#ifndef AMOUNT_HPP_
#define AMOUNT_HPP_


#include "vpiotr-decimal_for_cpp/vpiotr-decimal.h"
#include "Currency.hpp"

//typedef double money_amount_internal_representation;
//typedef float money_amount_internal_representation;
#define MONEY_DECIMALS 2
typedef dec::decimal<MONEY_DECIMALS> money_amount_internal_representation;


class MoneyAmount
{
public:
	// no value constructor because it's too complicated to handle type conversions.
	// see dedicated function.
	MoneyAmount();
	MoneyAmount(money_amount_internal_representation value, const Currency& currency);
	MoneyAmount(double value, const Currency& currency);
	MoneyAmount(double value, std::string currency_code);
	virtual ~MoneyAmount(); // just for declaring it virtual

	template<typename T>
	void set_value(const T& value);

	/////// POD ///////
	MoneyAmount operator+(const MoneyAmount&);
	MoneyAmount operator-(const MoneyAmount&);
	MoneyAmount operator*(const MoneyAmount&);
	MoneyAmount operator/(const MoneyAmount&);

	MoneyAmount operator +=(const MoneyAmount&);
	MoneyAmount operator -=(const MoneyAmount&);
	MoneyAmount operator *=(const MoneyAmount&);
	MoneyAmount operator /=(const MoneyAmount&);

	bool operator==(const MoneyAmount&) const;
	bool operator!=(const MoneyAmount&) const;
	bool operator<(const MoneyAmount&) const;
	bool operator<=(const MoneyAmount&) const;
	bool operator>(const MoneyAmount&) const;
	bool operator>=(const MoneyAmount&) const;
	/////// POD ///////

	Currency get_currency() const { return m_currency; }

	money_amount_internal_representation get_internal_value() const { return m_internal_value; }

	template<typename T>
	T get_value() const { return static_cast<T>(m_internal_value.getAsDouble()); }

	std::string get_human_representation() const;

protected:
	void init(money_amount_internal_representation value, const Currency& currency);

	static void check_operands_compatibility(const MoneyAmount& a, const MoneyAmount& b);

	money_amount_internal_representation m_internal_value;
	Currency m_currency;

	friend std::ostream& operator<<(std::ostream& os, const MoneyAmount&);
};

std::ostream& operator<<(std::ostream& os, const MoneyAmount&);


#endif /* AMOUNT_HPP_ */
