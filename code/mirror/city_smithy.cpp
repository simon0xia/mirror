#include "city_smithy.h"
#include <QTreeWidgetItem>

city_smithy::city_smithy(QWidget *parrent, RoleInfo *roleInfo)
	: QWidget(parrent), myRole(roleInfo)
{
	ui.setupUi(this);

//	ui.treeWidget->setStyleSheet("QTreeWidget { background:transparent} ");
	ui.treeWidget->setVisible(false);

// 	QString str[3] = { QStringLiteral("±¶ Ø"), QStringLiteral("ª§∂‹"), QStringLiteral("ªÍ÷È") };
// 
// 	for (int i = 0; i < 3; i++)
// 	{
// 		QTreeWidgetItem *prpoType = new QTreeWidgetItem(ui.treeWidget);
// 		prpoType->setText(0, str[i]);
// 	}
}

city_smithy::~city_smithy()
{

}

void city_smithy::on_btn_close_clicked(void)
{
	close();
}