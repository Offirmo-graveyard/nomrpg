#include <iostream>
//#include <typeinfo>

#include "off_spec/off_spec.hpp"

#include "RealUser.hpp"


#include "MoralPerson.hpp" // just to test it's not'

#ifdef USE_WT
#include "wt_spec_helpers.hpp"
#endif



DESCRIBE_CLASS(RealUser)
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

		SHOULD_EQ(o.get_class_name(), "RealUser");
		SHOULD_EQ(o.get_STI_type(),   "RealUser");
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
	}

	IT(is_persistable)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::Session& session = *(persistence_test_session->main_dbo_session());

		// 1) storing
		{
			Wt::Dbo::Transaction transaction(session);

			DescribedClass* p = new RealUser("Toto");
			Wt::Dbo::ptr<DescribedClass> pdbo = session.add(p);
			transaction.commit();
		}

		// 2) retrieving
		{
			Wt::Dbo::Transaction transaction(session);

			Wt::Dbo::ptr<DescribedClass> pdbo = session.find<RealUser>().where("denomination = ?").bind("Toto");
			std::cout << "Found RealUser with denomination : " << pdbo->get_denomination() << std::endl;
			transaction.commit();
		}

		// 3) iterating
		//   a) preparation
		{
			Wt::Dbo::Transaction transaction(session);

			DescribedClass* p = new RealUser("Titi");
			//p->set_email("titi@test.com");
			session.add(p);
			p = new RealUser("Tata");
			session.add(p);
			transaction.commit();
		}
		//   b) Iteration
		{
			Wt::Dbo::Transaction transaction(session);

			typedef Wt::Dbo::collection< Wt::Dbo::ptr<RealUser> > RealUsers;

			RealUsers users = session.find<RealUser>();
			std::cout << "We have " << users.size() << " real_users:" << std::endl;
			for (RealUsers::const_iterator i = users.begin(); i != users.end(); ++i)
				 std::cout << " real user with den. " << (*i)->get_denomination() << std::endl;
			transaction.commit();
		}

		// 4) modifying
		{
			Wt::Dbo::Transaction transaction(session);

			Wt::Dbo::ptr<RealUser> pdbo = session.find<RealUser>().where("denomination = ?").bind("Toto");
			// TODO
			//ru_ptr3->modify()->

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
