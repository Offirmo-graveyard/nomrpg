/*
#include "Currency.hpp"
 */
#ifndef CURRENCY_HPP_
#define CURRENCY_HPP_


#include <string>

// Note for future : nice csv here with all infos : http://mindprod.com/jgloss/currency.html

class Currency
{
public:
	static const std::string cm_default_currency_code;

	Currency(std::string ISO4217_code = Currency::cm_default_currency_code);
	virtual ~Currency(); // just for declaring it virtual

	std::string get_code() const { return m_ISO4217_code; };

	// useful
	bool operator==(const Currency& c) const { return m_ISO4217_code == c.m_ISO4217_code; }
	bool operator!=(const Currency& c) const { return m_ISO4217_code != c.m_ISO4217_code; }

protected:
	std::string m_ISO4217_code;
};

#endif /* CURRENCY_HPP_ */
