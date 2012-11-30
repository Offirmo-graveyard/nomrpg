/*
#include "debug_infos_page.h"
 */

#ifndef DEBUG_INFOS_PAGE_HPP_INCLUDED__
#define DEBUG_INFOS_PAGE_HPP_INCLUDED__


#include <Wt/WContainerWidget>

/*
 * A page displaying a lot of informations,
 * useful for debug.
 */
class DebugInfosPage : public Wt::WContainerWidget
{
public:
	DebugInfosPage(Wt::WApplication& parentApp);

protected:
	// utility functions for adding key/value informations
	void addLine(const std::string &text);
	void addKeyValueLine(const std::string &key, const std::string &value);
	void addKeyValueLine(const std::string &key, bool value);
	void addKeyValueLine(const std::string &key, double value);
};


#endif // DEBUG_INFOS_PAGE_HPP_INCLUDED__
