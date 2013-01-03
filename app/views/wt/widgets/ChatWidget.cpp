#include "ChatWidget.hpp"

ChatWidget::ChatWidget(boost::shared_ptr<AppSession> app_session):
	OffirmoTemplatedWidget(app_session)
{
	init_from_last_child();
}
