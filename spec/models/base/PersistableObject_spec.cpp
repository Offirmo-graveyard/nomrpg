#include <iostream>
#include <typeinfo>

#include "off_spec/off_spec.hpp"
#include "off_spec/off_spec_wt_model.hpp"

#include "PersistableObject.hpp"

#ifdef USE_WT
#include "wt_spec_helpers.hpp"
#endif


// an id for test
static const int sc_test_value = 333;

/* The PersistableObject class is pure virtual.
 * We need to derive from it.
 */
class BadPersistableObjectForTest : public PersistableObject
{
public:
	virtual const std::string get_class_name() const { return "BadPersistableObjectForTest"; }
};

class PersistableObjectForTest : public PersistableObject
{
public:
	PersistableObjectForTest(int some_value = 0):
		PersistableObject(),
		m_some_value(some_value)
	{ m_STI_type = get_class_name(); }

	int get_value() const { return m_some_value; }

	//////////////////////////////////// DBO ////////////////////////////////////
	// for STI (Single Type Inheritance). It MUST be equals to the class name.
	virtual const std::string get_class_name() const { return "PersistableObjectForTest"; }
#ifdef USE_WT
	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a,     m_some_value,  "some_value");

		PersistableObject::persist_virtual(a);
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

protected:
	int m_some_value;
};

DESCRIBE_CLASS(PersistableObject)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		PersistableObjectForTest o;
	}

	IT(has_correct_STI_info)
	{
		TRACE_CURRENT_TEST();

		PersistableObjectForTest o;

		//std::cout << "  typeid = " << typeid(o).name() << ", class name = " << o.get_class_name() << std::endl;

		SHOULD_BE_A(Object, o);
		SHOULD_EQ(o.get_class_name(), "PersistableObjectForTest");
		SHOULD_EQ(o.get_STI_type(),   "PersistableObjectForTest");

		BadPersistableObjectForTest bo;
		SHOULD_EQ(bo.get_class_name(), "BadPersistableObjectForTest");
		SHOULD_EQ(bo.get_STI_type(),   "");
	}

	IT(has_correct_creation_and_modification_date_fields)
	{
		TRACE_CURRENT_TEST();

		boost::posix_time::ptime current_test_start_datetime = boost::posix_time::microsec_clock::universal_time();

		PersistableObjectForTest o;

		//float close_dist = off_spec_max_distance_considered_close(current_test_start_datetime);
		//float dist = off_spec_distance(current_test_start_datetime, o.get_creation_date());

		boost::posix_time::ptime creation_date_backup = o.get_creation_date(); // to check if it moves after creation (it shouldn't !)

		SHOULD_EQ(o.get_creation_date(), creation_date_backup); // of course
		SHOULD_BE_FALSE(o.get_creation_date().is_not_a_date_time());
		SHOULD_BE_CLOSE(current_test_start_datetime, o.get_creation_date());

		SHOULD_BE_TRUE(o.get_last_modification_date().is_not_a_date_time());

		// normally called automatically but within a subclass registered in base
		o.register_modification();

		SHOULD_EQ(o.get_creation_date(), creation_date_backup); // creation date should not have changed
		SHOULD_BE_FALSE(o.get_last_modification_date().is_not_a_date_time());
		SHOULD_BE_CLOSE(current_test_start_datetime, o.get_last_modification_date());
		SHOULD_BE_GREATER_OR_EQUAL_THAN(o.get_creation_date(), o.get_last_modification_date());
	}

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
				session->mapClass<PersistableObjectForTest>("persistable_object_for_test");
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

			PersistableObjectForTest* p = new PersistableObjectForTest(sc_test_value);

			Wt::Dbo::ptr<PersistableObjectForTest> pdbo = session.add(p);

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

			Wt::Dbo::ptr<PersistableObjectForTest> pdbo = session.find<PersistableObjectForTest>().where("some_value = ?").bind(sc_test_value);
			std::cout << "Found PersistableObjectForTest with denomination : " << pdbo->get_value() << std::endl;
			SHOULD_EQ(pdbo->get_value(), sc_test_value);

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
