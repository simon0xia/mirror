#include "bag_item.h"

bag_item::bag_item(QMap<quint32, quint32> *item)
	: QWidget(NULL)
	, m_item(item)
{
	ui.setupUi(this);

	ui.tableWidget->setRowCount(3);
	ui.tableWidget->setColumnCount(3);

	testBag();
}

bag_item::~bag_item()
{

}

void bag_item::testBag()
{
	for (quint32 i = 0; i < 13; i++)
	{
		m_item->insert(200000 + i, qrand() % 100);
	}
	
	return;
}