#include "fight_map.h"
#include <QTime>
#include <QFile>
#include <QMessageBox>

extern QWidget *g_widget;

fight_map::fight_map(qint32 mapID, RoleInfo *info, MapItem *bag_item, ListEquip *bag_equip)
	: QWidget(NULL), m_mapID(mapID), myRole(info), m_bag_item(bag_item), m_bag_equip(bag_equip)
{
	ui.setupUi(this);
	ui.listWidget->setMovement(QListView::Static);

	QString strTmp;
	QString iconPath = ":/map/Resources/map/";

	m_dlg_fightfight = nullptr;

	const mirror_map *p = map_minor[mapID];
	for (qint32 i = 0; i < mapCount_minor; i++)
	{
		if (!p->name.isEmpty())
		{
			strTmp = iconPath + QString::number(p->imageID) + ".png";
			if (!QFile::exists(strTmp))
			{
				strTmp = ":/map/Resources/map/000.png";
			}
			QListWidgetItem *item = new QListWidgetItem(QIcon(strTmp), p->name);
			ui.listWidget->addItem(item);
			++p;
		}
	}

	connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemClicked(QListWidgetItem *)));
}

fight_map::~fight_map()
{

}

void fight_map::itemClicked(QListWidgetItem * item)
{
	const mirror_map p = map_minor[m_mapID][ui.listWidget->currentRow()];
	
	m_dlg_fightfight = new fight_fight(g_widget, m_mapID * 10 + p.ID, myRole, m_bag_item, m_bag_equip);
	m_dlg_fightfight->setWindowFlags(Qt::SubWindow);
	m_dlg_fightfight->move(g_widget->mapFromGlobal(g_widget->pos()) + QPoint(8, 30));
	m_dlg_fightfight->exec();

	delete m_dlg_fightfight;
	m_dlg_fightfight = nullptr;
}