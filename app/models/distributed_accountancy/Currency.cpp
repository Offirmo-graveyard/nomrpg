#include "Currency.hpp"

const std::string Currency::cm_default_currency_code = "EUR";


Currency::Currency(std::string ISO4217_code):
	m_ISO4217_code(ISO4217_code)
{
}

Currency::~Currency()
{
}
