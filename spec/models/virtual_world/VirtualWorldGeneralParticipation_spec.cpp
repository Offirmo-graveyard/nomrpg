/*
 * virtual_world_generic_participation_specs.cpp
 *
 *  Created on: May 16, 2012
 *      Author: admyn
 */

#include <iostream>
#include <typeinfo>

#include "off_spec/off_spec.hpp"

#include "VirtualWorldGeneralParticipation.hpp"

#ifdef USE_WT
#include "wt_spec_helpers.hpp"
#endif



DESCRIBE_CLASS(VirtualWorldGeneralParticipation)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o("", "");
	}

	IT(has_correct_STI_info)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o("", "");

		//std::cout << "  typeid = " << typeid(o).name() << ", class name = " << o.get_class_name() << std::endl;

		SHOULD_EQ(o.get_class_name(), "VirtualWorldGeneralParticipation");
		//SHOULD_EQ(o.get_STI_type(),   "VirtualWorldGeneralParticipation");
	}

	IT(is_tied_to_a_virtual_world)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o("", "");

		//SHOULD_BE(natural, o);
		//SHOULD_NOT_BE(moral, o);
	}
#if 0
	IT(has_proper_inheritance)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o("", "");

		SHOULD_BE_A(NaturalPerson, o);
	}

	IT(is_persistable)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::Session& session = SpecHelp::Wt::get_dbo_session();

		// Storing
		{
			Wt::Dbo::Transaction transaction(session);

			DescribedClass* p = new Avatar("MrToto");

			Wt::Dbo::ptr<DescribedClass> pdbo = session.add(p);

			transaction.commit();
		}
	}

	IT(is_unpersistable)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::Session& session = SpecHelp::Wt::get_dbo_session();

		// retrieving
		{
			Wt::Dbo::Transaction transaction(session);

			Wt::Dbo::ptr<Avatar> pdbo = session.find<Avatar>().where("denomination = ?").bind("MrToto");
			std::cout << "Found Avatar with denomination : " << pdbo->get_denomination() << std::endl;
			SHOULD_EQ(pdbo->get_denomination(), "MrToto");

			transaction.commit();
		}
	}
#endif
}
