#include "DistributedAccount.hpp"

#include <iostream>

#include "off_spec/off_spec.hpp"

#include "Currency.hpp"

#ifdef USE_WT
#include "wt_spec_helpers.hpp"
#endif


DESCRIBE_CLASS(DistributedAccount)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;
	}

	IT(has_a_currency)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		SHOULD_NOT_BE_EMPTY(o.get_currency().get_code());
	}

	IT(has_a_main_sub_account)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		SHOULD_NOT_BE_EMPTY(o.get_currency().get_code());
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
