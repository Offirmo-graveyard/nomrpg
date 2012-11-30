/*
#include "WtDboData.hpp"
 */

#ifndef WT_DBO_DATA_HPP_
#define WT_DBO_DATA_HPP_


#include <string>

#include <boost/shared_ptr.hpp>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/SqlConnection>
#include <Wt/Dbo/Session>


/* A small utility class
 * to dynamically add table definitions
 * to the default ones (useful in unit tests)
 */
class WtDboDataSchemaExtension
{
public:
	virtual ~WtDboDataSchemaExtension() {} // just for declaring it virtual and remove warning

	virtual void map_additional_classes(boost::shared_ptr<Wt::Dbo::Session> session) const {} // easier than pure virtual
};
/** A small container class
 *  for several related Wt Dbo data.
 */
struct WtDboData
{
	std::string info_str;
	boost::shared_ptr<Wt::Dbo::SqlConnection> cnx;
	boost::shared_ptr<Wt::Dbo::Session> session;

	WtDboData();
	virtual ~WtDboData();

	/*
	 * possible parameter :
	 * - ":memory:"
	 * - "xyz.db"
	 */
	void init_for_sqllite(std::string info_str = "database.db");

	// init db schema if needed.
	// should this func really be here ?
	void ensure_db_schema();
	void ensure_db_schema(const WtDboDataSchemaExtension& ext);

	// close and clean everything possible
	// note : use it for testing purpose only
	void close();

	// utility for deleting test sqlite files
	static void delete_file_if_it_exists(std::string file);
};

#endif /* WT_DBO_DATA_HPP_ */
