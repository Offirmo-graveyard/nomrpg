#include <iostream>
#include <typeinfo>

#include "off_spec/off_spec.hpp"

#include "VirtualWorldInstanceParticipation.hpp"

#ifdef USE_WT
#include "off_spec/off_spec_wt_model.hpp"
#include "wt_spec_helpers.hpp"
static const std::string TEST_NAME_VWI = "Beta Server";
#endif


DESCRIBE_CLASS(VirtualWorldInstanceParticipation)
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

		SHOULD_EQ(o.get_class_name(), "VirtualWorldInstanceParticipation");
		//SHOULD_EQ(o.get_STI_type(),   "VirtualWorldInstance");
	}

	IT(has_correct_starting_values)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;

		// check initial state
	}

#if 0 //def USE_WT
	IT(has_a_Wt_session)
	{
		TRACE_CURRENT_TEST();

		SpecHelp::OffirmoWebApp::ensure_basic_working_session();
	}

	IT(is_persistable)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::Session& session = *SpecHelp::Wt::s_app_session->main_dbo_session();

		// 1) Storing
		{
			Wt::Dbo::Transaction transaction(session);

			DescribedClass* p = new DescribedClass(TEST_NAME_VWI);
			Wt::Dbo::ptr<DescribedClass> pdbo = session.add(p);

			transaction.commit();
		}
	}

	IT(is_unpersistable)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::Session& session = *SpecHelp::Wt::s_app_session->main_dbo_session();

		// retrieving
		{
			Wt::Dbo::Transaction transaction(session);

			Wt::Dbo::ptr<DescribedClass> pdbo = session.find<DescribedClass>().where("denomination = ?").bind(TEST_NAME_VWI);
			std::cout << "Found VirtualWorldInstanceParticipation with name : " << pdbo->get_name() << std::endl;
			SHOULD_EQ(pdbo->get_name(), TEST_NAME_VWI);

			transaction.commit();
		}
	}
#endif // USE_WT
}
