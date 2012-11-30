/*
#include "mok_orm.hpp"
*/

#ifndef MOK_ORM_H_INCLUDED__
#define MOK_ORM_H_INCLUDED__


namespace mok
{
namespace orm
{

#ifndef MOK_ORM
#   define MOK_ORM_DECLARATIONS_BEGIN
#   define MOK_ORM_DECLARATIONS_END
#else


#if MOK_ORM==WT
#include <typeinfo> // for a hack
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/WtSqlTraits>
//#include <Wt/Dbo/DbAction> // for a hack
//class Wt::Dbo::SessionAddAction;

namespace dbo = Wt::Dbo;

#   define MOK_ORM_DECLARATIONS_BEGIN
#   define MOK_ORM_DECLARATIONS_END

#else
 // TODO
#endif

template<class Action, typename field_type>
void declare_field(Action& a, field_type field, const std::string& field_name)
{
//dbo::field(a, m_creation_date,   "creation_date");
}

template<class Action, typename field_type>
void redeclare_id(Action& a, field_type field, const std::string& field_name, int size)
{
//dbo::id(a, m_denomination, "denomination", NamedPersistableObject::MAX_DENOMINATION_SIZE);
}

#endif

} // namespace orm
} // namespace mok


#endif // MOK_ORM_H_INCLUDED__
