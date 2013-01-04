#include <iostream>
#include <cmath>
#include <map>

#include "off_spec/off_spec.hpp"

#include "MoneyAmount.hpp"

#ifdef USE_WT
#include "wt_spec_helpers.hpp"
#endif


DESCRIBE_CLASS(MoneyAmount)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;
	}

	IT(has_a_value_and_a_currency)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		// this should just compile
		(void)o.get_currency();
		(void)o.get_value<float>();
	}

	IT(has_correct_default_values)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		SHOULD_NOT_BE_EMPTY(o.get_currency().get_code());
		SHOULD_EQ(o.get_currency().get_code(), MoneyAmount::cm_default_incorrect_currency_code); // explicitly wrong value to detect errors
		SHOULD_BE_FALSE(std::islessgreater(o.get_value<float>(), 0.0f));
		std::cout << o.get_human_representation() << std::endl;
		SHOULD_EQ(o.get_human_representation(), "0.00?");
	}

	IT(has_a_human_decimal_representation)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o1(0, "EUR");
		SHOULD_EQ(o1.get_human_representation(), "0.00EUR");

		DescribedClass o2(1, "EUR");
		SHOULD_EQ(o2.get_human_representation(), "1.00EUR");

		DescribedClass o3(1.01, "EUR");
		SHOULD_EQ(o3.get_human_representation(), "1.01EUR");

		DescribedClass o4(0.075, "EUR");
		//std::cout << "  o4 = " << o4.get_human_representation() << std::endl;
		SHOULD_EQ(o4.get_human_representation(), "0.08EUR");

		DescribedClass o4b(0.074, "EUR");
		//std::cout << "  o4b = " << o4b.get_human_representation() << std::endl;
		SHOULD_EQ(o4b.get_human_representation(), "0.07EUR");

		DescribedClass o5(100000.89, "EUR");
		SHOULD_EQ(o5.get_human_representation(), "100000.89EUR");
	}

	IT(handle_decimal_arithmetic_correctly)
	{
		TRACE_CURRENT_TEST();

		{
			// simple test
			DescribedClass a(25.35, "USD");
			DescribedClass b(35.75, "USD");
			DescribedClass res = a + b;
			//std::cout << "  $25.35 + $35.75 -> " << res.get_value<float>() << " / " << res.get_human_representation() << std::endl;
			SHOULD_BE_FALSE(std::islessgreater(res.get_value<float>(), 61.1f));
			SHOULD_EQ(res.get_human_representation(), "61.10USD");
		}

		{
			// excellent example given by Microsoft (hat tip)
			// http://support.microsoft.com/kb/214118
			// 1*(.5-.4-.1)
			// double and float fail this test
			/*double a = 1;
			DescribedClass b(.5, "EUR");
			DescribedClass c(.4, "EUR");
			DescribedClass d(.1, "EUR");
			DescribedClass res = a * ( b - c - d );

			//std::cout << "  1*(.5-.4-.1) -> " << res.get_value<float>() << " / " << res.get_human_representation() << std::endl;
			SHOULD_BE_FALSE(std::islessgreater(res.get_value<float>(), 0.0f));
			SHOULD_EQ(res.get_human_representation(), "0.00");*/
		}
	}

	IT(has_a_correct_comparison_operator)
	{
		TRACE_CURRENT_TEST();

		DescribedClass a1(1, "EUR");
		DescribedClass a2(1, "EUR");
		DescribedClass b1(1, "USD");
		DescribedClass b2(1, "USD");
		DescribedClass c1(2, "EUR");
		DescribedClass c2(2, "EUR");
		DescribedClass d1(2, "USD");
		DescribedClass d2(2, "USD");

		SHOULD_BE_TRUE(a1 == a2);
		SHOULD_BE_TRUE(b1 == b2);
		SHOULD_BE_TRUE(c1 == c2);
		SHOULD_BE_TRUE(d1 == d2);
		SHOULD_BE_FALSE(a1 != a2);
		SHOULD_BE_FALSE(b1 != b2);
		SHOULD_BE_FALSE(c1 != c2);
		SHOULD_BE_FALSE(d1 != d2);

		SHOULD_BE_FALSE(a1 == b1);
		SHOULD_BE_FALSE(a1 == c1);
		SHOULD_BE_FALSE(a1 == d1);
		SHOULD_BE_TRUE(a1 != b1);
		SHOULD_BE_TRUE(a1 != c1);
		SHOULD_BE_TRUE(a1 != d1);
	}

	IT(checks_explicit_currencies_compatibility)
	{
		TRACE_CURRENT_TEST();

		DescribedClass a(1, "EUR");
		DescribedClass b(1, "USD");

		SHOULD_THROW(Mok::IncoherentDataException, a + b);
		SHOULD_THROW(Mok::IncoherentDataException, a - b);
		SHOULD_THROW(Mok::IncoherentDataException, b + a);
		SHOULD_THROW(Mok::IncoherentDataException, b - a);
		SHOULD_THROW(Mok::IncoherentDataException, a += b);
		SHOULD_THROW(Mok::IncoherentDataException, a -= b);
	}

	IT(allows_default_constructed_objects)
	{
		TRACE_CURRENT_TEST();

		DescribedClass a;
		DescribedClass b(1, "USD");
		DescribedClass c;

		// OK because a is default constructed
		SHOULD_EQ(a + b, MoneyAmount( 1, "USD"));
		SHOULD_EQ(a - b, MoneyAmount(-1, "USD"));
		SHOULD_THROW(Mok::IncoherentDataException, a - c);

		std::map<int, MoneyAmount> map;
		map[0] += b;
		map[1] -= b;
		SHOULD_EQ(map[0], MoneyAmount( 1, "USD"));
		SHOULD_EQ(map[1], MoneyAmount(-1, "USD"));

		a.set_value(1); // a is no longer default constructed
		SHOULD_THROW(Mok::IncoherentDataException, a + b);
		SHOULD_THROW(Mok::IncoherentDataException, a - b);
	}

	IT(should_have_robust_operators)
	{
		TRACE_CURRENT_TEST();

		DescribedClass a(2, "EUR");

		SHOULD_EQ(a, MoneyAmount( 2, "EUR"));

		a = a; // classic
		SHOULD_EQ(a, MoneyAmount( 2, "EUR"));

		SHOULD_EQ(a + a, MoneyAmount( 4, "EUR"));
		// REM : a is not modified and still = 2EUR

		SHOULD_EQ(a += a, MoneyAmount( 4, "EUR"));
		SHOULD_EQ(a, MoneyAmount( 4, "EUR"));

		SHOULD_EQ(a - a, MoneyAmount( 0, "EUR"));
		// REM : a is not modified and still = 4EUR

		SHOULD_EQ(a -= a, MoneyAmount( 0, "EUR"));
		SHOULD_EQ(a, MoneyAmount( 0, "EUR"));
	}

#if 0 //def USE_WT
	boost::shared_ptr<AppSession> persistence_test_session;
	IT(has_a_Wt_session)
	{
		TRACE_CURRENT_TEST();

		persistence_test_session = SpecHelp::create_dbo_session();
		SpecHelp::fill_session_up_to_vwi(persistence_test_session);
	}

	IT(is_persistable)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::Session& session = *(persistence_test_session->main_dbo_session());

		// Storing
		{
			Wt::Dbo::Transaction transaction(session);

			DescribedClass* p = new Avatar(*(persistence_test_session->real_user()), *(persistence_test_session->instance()), "MrToto");

			Wt::Dbo::ptr<DescribedClass> pdbo = session.add(p);

			transaction.commit();
		}
	}

	IT(is_unpersistable)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::Session& session = *(persistence_test_session->main_dbo_session());

		// retrieving
		{
			Wt::Dbo::Transaction transaction(session);

			Wt::Dbo::ptr<Avatar> pdbo = session.find<Avatar>().where("denomination = ?").bind("MrToto");
			std::cout << "Found Avatar with denomination : " << pdbo->get_denomination() << std::endl;
			SHOULD_EQ(pdbo->get_denomination(), "MrToto");

			transaction.commit();
		}
	}

	IT(no_longer_has_a_Wt_session)
	{
		TRACE_CURRENT_TEST();

		persistence_test_session.reset();
	}
#endif // USE_WT
}
