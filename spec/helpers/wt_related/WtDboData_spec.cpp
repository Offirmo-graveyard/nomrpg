#include <iostream>

#include "off_spec/off_spec.hpp"

#include "WtDboData.hpp"


DESCRIBE_CLASS(WtDboData)
{
	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		DescribedClass o;
	}

	IT(can_be_set_to_a_sqlite_file)
	{
		TRACE_CURRENT_TEST();

		std::string db_file_name = "WtDboData_test.db";

		WtDboData dbo_data;

		dbo_data.init_for_sqllite(db_file_name);
		dbo_data.ensure_db_schema();

		// how to check for success ?

		// cleanup
		dbo_data.close();
		dbo_data.delete_file_if_it_exists(db_file_name);
	}

	// TODO more

}
