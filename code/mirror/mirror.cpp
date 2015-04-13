#include "mirror.h"

QWidget *g_widget;

mirror::mirror(QWidget *parent)
	: QMainWindow(parent)
	, m_tab_role(&roleInfo, &m_bag_item, &m_storage_item)
	, m_tab_fight(&roleInfo)
	, m_tab_city(&roleInfo)
{
	ui.setupUi(this);

	g_widget = this;

	ui.tabWidget_main->addTab(&m_tab_fight, QString::fromLocal8Bit("战斗"));
	ui.tabWidget_main->addTab(&m_tab_role, QString::fromLocal8Bit("角色"));
	ui.tabWidget_main->addTab(&m_tab_city, QString::fromLocal8Bit("城市"));

	ui.tabWidget_main->setCurrentIndex(1);
	
	connect(ui.tabWidget_main, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}

mirror::~mirror()
{

}

void mirror::tabChanged(int index)
{
	QWidget *tab = ui.tabWidget_main->currentWidget();
	myTabFrame *frame = (myTabFrame *)tab;
	if (frame != NULL)
	{
		frame->updateRoleInfo();
	}
}