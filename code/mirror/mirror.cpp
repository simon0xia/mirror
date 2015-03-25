#include "mirror.h"


mirror::mirror(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.tabWidget_main->addTab(&m_tab_fight, QString::fromLocal8Bit("战斗"));
	ui.tabWidget_main->addTab(&m_tab_role, QString::fromLocal8Bit("角色"));
	ui.tabWidget_main->addTab(&m_tab_city, QString::fromLocal8Bit("城市"));
}

mirror::~mirror()
{

}
