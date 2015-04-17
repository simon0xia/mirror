#include "fight_map.h"
#include <QTime>

extern QWidget *g_widget;

fight_map::fight_map(qint32 mapID, RoleInfo *info, MapItem *bag_item)
: QWidget(NULL), m_mapID(mapID), myRole(info), m_bag_item(bag_item)
{
	ui.setupUi(this);

	//暂不开放2号(盟重）以上地图。
	if (m_mapID > 2)
	{
		return;
	}

	QString strTmp, strBoss;
	QString iconPath = ":/map/Resources/map/";
	if (m_mapID < 10)
	{
		iconPath += "0";
	}
	iconPath += QString::number(m_mapID);

	const mirror_map *p = map_minor[mapID];
	for (qint32 i = 0; i < mapCount_minor; i++)
	{
		if (!p->name.isEmpty())
		{
			qsrand(QTime::currentTime().msec());
			bHasBoss[i] = (qrand() % 10) > 8;						//10%概率刷BOSS----未实现

			strTmp = iconPath + QString::number(i) + ".png";
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
	const mirror_map *p = map_minor[m_mapID];
	for (qint32 i = 0; i < 9; i++)
	{
		if (p->name == item->text())
		{
			break;
		}
		p++;
	}
	
	fight_fight *fightfight = new fight_fight(g_widget, m_mapID * 10 + p->ID, myRole, m_bag_item);
	fightfight->setWindowFlags(Qt::SubWindow);
	fightfight->move(g_widget->mapFromGlobal(g_widget->pos()) + QPoint(8,30));
	fightfight->show();
}