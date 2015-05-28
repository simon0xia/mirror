#include "fight_map.h"
#include <QTime>
#include <QFile>
#include <QMessageBox>

extern QWidget *g_widget;
extern QVector<Info_Distribute> g_MonsterDistribute;

fight_map::fight_map(qint32 mapID, RoleInfo *info, MapItem *bag_item, ListEquip *bag_equip)
	: QWidget(NULL), m_mapID(mapID), myRole(info), m_bag_item(bag_item), m_bag_equip(bag_equip)
{
	ui.setupUi(this);
	ui.listWidget->setMovement(QListView::Static);
	m_dlg_fightfight = nullptr;

	deleyTimer = startTimer(1000);
	connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemClicked(QListWidgetItem *)));
}

fight_map::~fight_map()
{

}

void fight_map::timerEvent(QTimerEvent *event)
{
	killTimer(deleyTimer);

	qint32 nStart = m_mapID * 1000;
	qint32 nStop = (m_mapID + 1) * 1000;
	for (QVector<Info_Distribute>::const_iterator iter = g_MonsterDistribute.begin(); iter != g_MonsterDistribute.end(); iter++)
	{
		if (iter->ID < nStart)
		{
			continue;
		}
		else if (iter->ID < nStop)
		{
			if (myRole->level - 50 > iter->need_lv )
			{
				ui.listWidget->addItem(new QListWidgetItem(iter->img, iter->name));
			}
		}
		else
		{
			break;
		}
	}
}

void fight_map::itemClicked(QListWidgetItem * item)
{	
	qint32 map = m_mapID * 1000 + ui.listWidget->currentRow() + 1;

	m_dlg_fightfight = new fight_fight(g_widget, map, myRole, m_bag_item, m_bag_equip);
	m_dlg_fightfight->setWindowFlags(Qt::SubWindow);
	m_dlg_fightfight->move(g_widget->mapFromGlobal(g_widget->pos()) + QPoint(8, 30));
	m_dlg_fightfight->exec();

	delete m_dlg_fightfight;
	m_dlg_fightfight = nullptr;
}