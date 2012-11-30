#include <iostream>
#include <typeinfo>

#include "off_spec/off_spec.hpp"
#include "off_spec/off_spec_wt_model.hpp"

#ifdef USE_WT
#include "wt_spec_helpers.hpp"
#endif

#include "VirtualWorld.hpp"


static const std::string TEST_NAME_VW = "Very Own Testing Grounds";


DESCRIBE_CLASS(VirtualWorld)
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

		//std::cout << "  typeid = " << typeid(o).name() << ", class name = " << o.get_class_name() << " [" << o.get_STI_type() << "]" << std::endl;

		SHOULD_EQ(o.get_class_name(), "VirtualWorld");
		SHOULD_EQ(o.get_STI_type(),   "VirtualWorld");
	}

	IT(has_correct_default_values)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		// check initial state
		SHOULD_EQ(o.get_name(), VirtualWorld::cm_default_name);
		SHOULD_NOT_EQ(o.get_name(), "");
	}

	IT(has_correct_starting_values)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o(TEST_NAME_VW);

		// check initial state
		SHOULD_EQ(o.get_name(), TEST_NAME_VW);
	}

#ifdef USE_WT
	boost::shared_ptr<AppSession> persistence_test_session;
	IT(has_a_Wt_session)
	{
		TRACE_CURRENT_TEST();

		persistence_test_session = SpecHelp::create_dbo_session();
	}

	IT(is_persistable)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::Session& session = *(persistence_test_session->main_dbo_session());

		// 1) Storing
		{
			Wt::Dbo::Transaction transaction(session);

			DescribedClass* p = new VirtualWorld(TEST_NAME_VW);
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

			Wt::Dbo::ptr<DescribedClass> pdbo = session.find<VirtualWorld>().where("denomination = ?").bind(TEST_NAME_VW);
			std::cout << "Found VirtualWorld with name : " << pdbo->get_name() << std::endl;
			SHOULD_EQ(pdbo->get_name(), TEST_NAME_VW);

			transaction.commit();
		}
	}

	IT(cant_have_the_same_name_as_another_existing_VW)
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

	IT(has_a_singleton_like_accessor)
	{
		TRACE_CURRENT_TEST();

		// TODO design in progress

		//dbo::Session& session = *SpecHelp::Wt::s_app_session->main_dbo_session();

		// retrieving : should return previously created world
		/*dbo::ptr<DescribedClass> pdbo = VirtualWorld::get_current_one(session);
		SHOULD_EQ(pdbo->get_name(), "Testing Grounds");

		// if no virtual world exists, it should create one
		// first remove the existing one
		pdbo.remove();
		// then ask for the current world : a default one will be recreated
		pdbo = VirtualWorld::get_current_one(session);
		// since it's a default one, it will have default name
		SHOULD_EQ(pdbo->get_name(), VirtualWorld::cm_default_name);*/
	}
}
