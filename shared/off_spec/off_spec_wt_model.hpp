/* Specs expectations
 * specific to Wt objects (dates, DBO)
 */
/*
#include "off_spec_wt_model.hpp"
 */

#ifndef OFF_SPEC_WT_MODEL_HPP_INCLUDED__
#define OFF_SPEC_WT_MODEL_HPP_INCLUDED__


#include "off_spec.hpp"

#ifdef USE_WT

#include <Wt/WDateTime>

template<>
float off_spec_max_distance_considered_close(const Wt::WDateTime& useless_param_just_for_easy_templating);

template<>
float off_spec_distance(const Wt::WDateTime& elem1, const Wt::WDateTime& elem2);

// REM : no () for those params
#define SHOULD_THROW_WT_EXCEPTION(expected_wt_dbo_exception_code, expression)   \
	do \
	{ \
		bool caught_ = false; \
		bool correct_code_ = false; \
		try { expression; } \
		catch (const Wt::Dbo::Exception& ex) { \
			caught_ = true; \
			std::cout << "msg = '" << ex.code() << "'" << std::endl; \
			correct_code_ = (ex.code() == expected_wt_dbo_exception_code); \
		} catch (...) {} \
		if (!caught_) \
			UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), "Expected Wt Dbo exception not thrown"); \
		else if (!correct_code_) \
			UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), "Expected Wt Dbo exception message mismatch"); \
	} while(0)

#endif // USE_WT


#endif // OFF_SPEC_WT_MODEL_HPP_INCLUDED__
