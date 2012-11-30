#include <iostream>
#include <typeinfo>

#include "off_spec/off_spec.hpp"

#include "Avatar.hpp"

#ifdef USE_WT
#include "wt_spec_helpers.hpp"
#endif


DESCRIBE_CLASS(Avatar)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;
	}

	IT(has_correct_STI_info)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		//std::cout << "  typeid = " << typeid(o).name() << ", class name = " << o.get_class_name() << std::endl;

		SHOULD_EQ(o.get_class_name(), "Avatar");
		SHOULD_EQ(o.get_STI_type(),   "Avatar");
	}

	IT(responds_correctly_to_moral_or_natural)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		SHOULD_BE(natural, o);
		SHOULD_NOT_BE(moral, o);
	}

	IT(has_proper_inheritance)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		SHOULD_BE_A(NaturalPerson, o);
	}

#ifdef USE_WT
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

	IT(cant_have_the_same_name_as_another_existing_avatar_for_the_same_VW_instance)
	{
		TRACE_CURRENT_TEST();

		// TODO
	}

	IT(no_longer_has_a_Wt_session)
	{
		TRACE_CURRENT_TEST();

		persistence_test_session.reset();
	}
#endif // USE_WT
}
