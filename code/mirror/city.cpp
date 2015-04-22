#include "city.h"

city::city(RoleInfo *roleInfo, MapItem *bag_item)
: myTabFrame(NULL), myRole(roleInfo), m_bag_item(bag_item)
{
	ui.setupUi(this);

	m_drugs = new shop(0, myRole, m_bag_item);
	ui.tabWidget->addTab(m_drugs, QStringLiteral("药店"));

	m_variety = new shop(1, myRole, m_bag_item);
	ui.tabWidget->addTab(m_variety, QStringLiteral("杂货铺"));

	m_smithy = new smithy(myRole);
	ui.tabWidget->addTab(m_smithy, QStringLiteral("铁匠铺"));
	tabChanged(0);

	connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}

city::~city()
{
	delete m_drugs;
	delete m_smithy;
}

void city::updateRoleInfo(void)
{

}

void city::tabChanged(int index)
{
	QString iconPath = ":/city/Resources/city/";
	iconPath += QString::number(index + 1) + ".png";
	ui.lbl_img->setPixmap(QPixmap(iconPath));

	QString strInfo[10] = {
		QStringLiteral("妙手回春"),
		QStringLiteral("包罗万象"),
		QStringLiteral("装备强化"),
		QStringLiteral("转生")
	};
	ui.edit_display->setText(strInfo[index]);
}