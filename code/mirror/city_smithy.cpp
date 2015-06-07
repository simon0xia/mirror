#include "city_smithy.h"
#include <QTreeWidgetItem>

city_smithy::city_smithy(QWidget *parrent, RoleInfo *roleInfo)
	: QWidget(parrent), myRole(roleInfo)
{
	ui.setupUi(this);

//	ui.treeWidget->setStyleSheet("QTreeWidget { background:transparent} ");
//	ui.treeWidget->setVisible(false);

	QString str[3] = { QStringLiteral("宝石"), QStringLiteral("护盾"), QStringLiteral("魂珠") };

	for (int i = 0; i < 3; i++)
	{
		QTreeWidgetItem *propType = new QTreeWidgetItem(ui.treeWidget);
		propType->setText(0, str[i]);
		for (int j = 0; j < 12; j++)
		{
			QTreeWidgetItem *propItem = new QTreeWidgetItem(propType);
			propItem->setText(0, QStringLiteral("等待添加"));
		}
	}
}

city_smithy::~city_smithy()
{

}

void city_smithy::on_btn_close_clicked(void)
{
	close();
}