#include <QMessageBox>
#include "item_equipstorage.h"

extern QWidget *g_widget;
extern Dlg_Detail *g_dlg_detail;

Item_equipStorage::Item_equipStorage(CPlayer *w_player)
	: player(w_player)
{
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	CurrentPage = 1;
//	ui.edit_page_all->setText(QString::number(2));

	m_item = player->get_bag_equip(); 
	m_storageItem = player->get_storage_equip();

	connect(ui.tableWidget, SIGNAL(cellEntered(int, int)), this, SLOT(ShowItemInfo(int, int)));
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu(QPoint)));
}

Item_equipStorage::~Item_equipStorage()
{

}


void Item_equipStorage::updateInfo()
{
	quint32 row_Count = ui.tableWidget->rowCount();
	quint32 Col_Count = ui.tableWidget->columnCount();
	quint32 row_cur = 0;
	quint32 col_cur = 0;

	QString strTmp = "";
	quint32 ID, nCount;
	QString Name;

	//必须先清除背包显示，否则当前装备数量小于之前数量时会在最尾显示原装备的假像。
	ui.tableWidget->clear();
	for (ListEquip::const_iterator iter = m_storageItem->constBegin(); iter != m_storageItem->constEnd(); iter++)
	{
		const Info_basic_equip *EquipBasicInfo = GetEquipBasicInfo(iter->ID);
		if (EquipBasicInfo == nullptr)
		{
			continue;
		}
		ui.tableWidget->setItem(row_cur, col_cur++, new QTableWidgetItem(QIcon(EquipBasicInfo->icon), strTmp));
		if (col_cur >= Col_Count)
		{
			++row_cur;
			col_cur = 0;
		}
	}
}

void Item_equipStorage::ShowItemInfo(int row, int column)
{
	ShowItemInfo_equip(row, column, CurrentPage, m_storageItem);
}

void Item_equipStorage::ShowContextMenu(QPoint pos)
{
	g_dlg_detail->hide();

	quint32 index = GetCurrentCellIndex(CurrentPage);
	if (m_storageItem->size() != 0 && m_storageItem->size() >= index)
	{
		const Info_Equip equip = m_storageItem->at(index);

		if (m_item->size() >= g_bag_maxSize)
		{
			QString message = QStringLiteral("背包已满！");
			QMessageBox::critical(g_widget, QStringLiteral("提示"), message);
		}
		else
		{
			m_item->append(equip);
			m_storageItem->removeAt(index);

			emit UpdateEquipInfoSignals();
		}
	}
}