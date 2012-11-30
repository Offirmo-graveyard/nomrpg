#include <iostream>
#include <typeinfo>

#include "off_spec/off_spec.hpp"

#include "VirtualWorldInstance.hpp"

#ifdef USE_WT
#include "off_spec/off_spec_wt_model.hpp"
#include "wt_spec_helpers.hpp"
static const std::string TEST_NAME = "virtual_world_instance_specs";
#endif

#include "app_config.h"




DESCRIBE_CLASS(VirtualWorldInstance)
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

		SHOULD_EQ(o.get_class_name(), "VirtualWorldInstance");
		//SHOULD_EQ(o.get_STI_type(),   "VirtualWorldInstance");
	}

	IT(has_correct_starting_values)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		// check initial state
		SHOULD_EQ(o.get_name(), VirtualWorldInstance::cm_default_name);
		SHOULD_NOT_EQ(o.get_name(), "");
	}

#ifdef USE_WT
	boost::shared_ptr<AppSession> persistence_test_session;
	IT(has_a_Wt_session)
	{
		TRACE_CURRENT_TEST();

		persistence_test_session = SpecHelp::create_dbo_session("vwi_test.db");
		SpecHelp::fill_session_up_to_vw(persistence_test_session);
	}

	IT(is_persistable)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::Session& session = *(persistence_test_session->main_dbo_session());

		// 1) Storing
		{
			Wt::Dbo::Transaction transaction(session);

			DescribedClass* p = new DescribedClass(*(persistence_test_session->virtual_world()), TEST_NAME);
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

			Wt::Dbo::ptr<DescribedClass> pdbo = session.find<DescribedClass>().where("denomination = ?").bind(TEST_NAME);
			std::cout << "Found VirtualWorldInstance with name : " << pdbo->get_name() << std::endl;
			SHOULD_EQ(pdbo->get_name(), TEST_NAME);

			transaction.commit();
		}
	}

	IT(cant_have_the_same_name_as_another_existing_instance_for_the_same_VW)
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
