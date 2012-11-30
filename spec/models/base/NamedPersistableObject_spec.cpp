#include <iostream>
#include <typeinfo>

#include "off_spec/off_spec.hpp"
#include "off_spec/off_spec_wt_model.hpp"

#include "NamedPersistableObject.hpp"

#ifdef USE_WT
#include "wt_spec_helpers.hpp"
#endif


// values for test
static const std::string sc_test_name = "Garfield";
static const int sc_test_value = -333;

/* The NamedPersistableObject class is pure virtual.
 * We need to derive from it.
 */
class NamedPersistableObjectForTest : public NamedPersistableObject
{
public:
	NamedPersistableObjectForTest(const std::string& denomination = ""):
		NamedPersistableObject(denomination)
	{ m_STI_type = get_class_name(); }

	//////////////////////////////////// DBO ////////////////////////////////////
	// for STI (Single Type Inheritance). It MUST be equals to the class name.
	virtual const std::string get_class_name() const { return "NamedPersistableObjectForTest"; }
#ifdef USE_WT
	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a,     m_some_value,  "some_value");

		NamedPersistableObject::persist_virtual(a);
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

	int m_some_value; // public for esaier tests
};

// REM we need this also :
#ifdef USE_WT
// specialization of some ancestor methods
WT_DBO_NAMED_OBJECT_CHILD_TEMPLATE_SPECIALIZATION(NamedPersistableObjectForTest)
#endif // WT


DESCRIBE_CLASS(NamedPersistableObject)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		NamedPersistableObjectForTest o;
	}

	IT(has_correct_initial_state)
	{
		TRACE_CURRENT_TEST();

		NamedPersistableObjectForTest o1;
		SHOULD_EQ(o1.get_name(), "");

		NamedPersistableObjectForTest o2("toto");
		SHOULD_EQ(o2.get_name(), "toto");
	}

	IT(has_a_name_field)
	{
		TRACE_CURRENT_TEST();

		NamedPersistableObjectForTest o("Dupont");

		// check initial state
		SHOULD_EQ(o.get_name(), "Dupont");

		// and name cannot be changed
		CHECK_THROW(o.rename("Dupond"), const char*);

		// check that name has not changed
		SHOULD_EQ(o.get_name(), "Dupont");
	}

	IT(has_correct_STI_info)
	{
		TRACE_CURRENT_TEST();

		NamedPersistableObjectForTest o;

		//std::cout << "  typeid = " << typeid(o).name() << ", class name = " << o.get_class_name() << std::endl;

		SHOULD_BE_A(Object, o);
		SHOULD_EQ(o.get_class_name(), "NamedPersistableObjectForTest");
		SHOULD_EQ(o.get_STI_type(),   "NamedPersistableObjectForTest");
	}

	// check actual persistence...
#ifdef USE_WT
	boost::shared_ptr<AppSession> persistence_test_session;
	IT(has_a_Wt_session)
	{
		TRACE_CURRENT_TEST();

		// helper declares default classes, but not our test one.
		// we use the specially designed extension mechanism to declare it
		class WtDboDataSchemaExtensionForPersistableObjectTest : public WtDboDataSchemaExtension
		{
		public:
			virtual void map_additional_classes(boost::shared_ptr<Wt::Dbo::Session> session) const
			{
				session->mapClass<NamedPersistableObjectForTest>("named_persistable_object_for_test");
			}
		} ext;

		persistence_test_session = SpecHelp::create_dbo_session(ext); // will also map our special class
	}

	IT(is_persistable)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::Session& session = *(persistence_test_session->main_dbo_session());

		// Storing
		{
			Wt::Dbo::Transaction transaction(session);

			NamedPersistableObjectForTest* p = new NamedPersistableObjectForTest(sc_test_name);
			p->m_some_value = sc_test_value;

			Wt::Dbo::ptr<NamedPersistableObjectForTest> pdbo = session.add(p);

			// p can no longer be used
			p = NULL;

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

			Wt::Dbo::ptr<NamedPersistableObjectForTest> pdbo = session.find<NamedPersistableObjectForTest>().where("denomination = ?").bind(sc_test_name);
			std::cout << "Found NamedPersistableObjectForTest with denomination : " << pdbo->get_name() << std::endl;
			SHOULD_EQ(pdbo->get_name(), sc_test_name);
			SHOULD_EQ(pdbo->m_some_value, sc_test_value); // check if the additional field also got persisted properly

			transaction.commit();
		}
	}

	IT(cant_create_another_object_with_the_same_name_as_an_existing_one)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::Session& session = *(persistence_test_session->main_dbo_session());

		// Storing
		{
			Wt::Dbo::Transaction transaction(session);

			NamedPersistableObjectForTest* p = new NamedPersistableObjectForTest(sc_test_name);
			p->m_some_value = sc_test_value + 3; // to be sure that unique constraint is not on this additional field

			// should FAIL
			Wt::Dbo::ptr<NamedPersistableObjectForTest> pdbo = session.add(p);

			SHOULD_THROW(Wt::Dbo::Exception, transaction.commit() );


		}
	}

	IT(no_longer_has_a_Wt_session)
	{
		TRACE_CURRENT_TEST();

		persistence_test_session.reset();
	}
#endif // USE_WT
}
