#include "WtDboData.hpp"

#include <assert.h>

#include <boost/filesystem.hpp>

#include <Wt/Dbo/backend/Sqlite3>


// for db schema init
#include "RealUser.hpp"
#include "VirtualWorld.hpp"
#include "VirtualWorldGeneralParticipation.hpp"
#include "VirtualWorldInstance.hpp"
#include "VirtualWorldInstanceParticipation.hpp"
#include "Avatar.hpp"
#include "TBRPGGame.hpp"
#include "ArchetypeGroup.hpp"


WtDboData::WtDboData()
{
	// TODO Auto-generated constructor stub
	
}

WtDboData::~WtDboData()
{
	// TODO Auto-generated destructor stub
}

void WtDboData::delete_file_if_it_exists(std::string file)
{
	//db_file = "test_database.sqlite.db";

	std::cout << "- deleting file '" << file << "'..." << std::endl;
	if (boost::filesystem::exists(file))    // does it actually exist?
	{
		if (boost::filesystem::is_regular_file(file)) // is it a regular file?
		{
			std::cout << "  the file exists : '" << file << "', deleting it..." << std::endl;
			boost::filesystem::remove(file);
		}
	}
}

void WtDboData::init_for_sqllite(std::string p_info_str)
{
	this->info_str = p_info_str;

	cnx.reset( new Wt::Dbo::backend::Sqlite3(info_str) );
	cnx->setProperty("show-queries", "true");

	session.reset( new Wt::Dbo::Session() );
	session->setConnection(*cnx.get());
}

void WtDboData::ensure_db_schema()
{
	WtDboDataSchemaExtension empty; // declares no additional classes

	ensure_db_schema(empty);
}

void WtDboData::ensure_db_schema(const WtDboDataSchemaExtension& ext)
{
	// XXX TODO make the object listing automatic and no longer manual

	std::cout << __func__ << std::endl;

	/// register database entries
	// user-society model
	session->mapClass<RealUser>("real_user");

	// Virtual world model
	session->mapClass<VirtualWorld>("virtual_world");
	session->mapClass<VirtualWorldGeneralParticipation>("virtual_world_general_participation");
	session->mapClass<VirtualWorldInstance>("virtual_world_instance");
	session->mapClass<VirtualWorldInstanceParticipation>("virtual_world_instance_participation");
	session->mapClass<Avatar>("avatar");

	// games models
	session->mapClass<TBRPGGame>("boring_rpg_game");
	session->mapClass<ArchetypeGroup>("archetype_group");

	// misc (most likely unit tests)
	ext.map_additional_classes(session);

	/*
	 * Try to create the schema (will fail if already exists).
	 */
	try
	{
		session->createTables();
	}
	catch(...)
	{
		// most likely a table already exists, don't care
	}
}


void WtDboData::close()
{
	// first close the session
	assert(session.unique());
	session.reset(); // will free it if still unique

	// then close the connection
	assert(cnx.unique());
	cnx.reset(); // will free it if still unique
}

