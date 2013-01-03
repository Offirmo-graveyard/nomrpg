#include "OffirmoTemplatedWidget.hpp"


OffirmoTemplatedWidget::OffirmoTemplatedWidget(boost::shared_ptr<AppSession> app_session):
	m_app_session(app_session)
{
}

void OffirmoTemplatedWidget::init_from_last_child()
{
	load_or_create_model();
	update();
	bind_components();

	// set the template to use
	setTemplateText(tr(get_template_full_name()));
}
