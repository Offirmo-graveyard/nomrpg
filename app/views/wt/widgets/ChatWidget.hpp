/*
#include "ChatWidget.hpp"
 */

#ifndef CHATWIDGET_HPP_
#define CHATWIDGET_HPP_

#include "OffirmoTemplatedWidget.hpp"


class ChatWidget: public OffirmoTemplatedWidget
{
public:
	ChatWidget(boost::shared_ptr<AppSession> app_session);
};

#endif /* CHATWIDGET_HPP_ */
