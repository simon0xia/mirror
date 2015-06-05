#include "fight_map.h"
#include <QTime>
#include <QFile>
#include <QMessageBox>

extern QWidget *g_widget;
extern QMap<mapID, Info_Distribute> g_MonsterDistribute;

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

	qint32 nStart = (m_mapID) * 1000;
	qint32 nStop = (m_mapID + 1) * 1000;
	QListWidgetItem *item;
	foreach(const Info_Distribute &dis, g_MonsterDistribute)
	{
		if (dis.ID < nStart)
		{
			continue;
		}
		else if (dis.ID < nStop)
		{
			if (myRole->level + 50 > dis.need_lv)
			{
				item = new QListWidgetItem(dis.img, dis.name);
				item->setWhatsThis(QString::number(dis.ID));
				ui.listWidget->addItem(item);
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
	mapID id = item->whatsThis().toUInt();

	int nNeedLv = g_MonsterDistribute[id].need_lv;
	if (myRole->level < nNeedLv)
	{
		QString message = QStringLiteral("勇士，你现在的实力不足以进入此地！\n升到%1级之后再来吧。").arg(nNeedLv);
		QMessageBox::critical(this, QStringLiteral("提示"), message);
		return;
	}
	
	m_dlg_fightfight = new fight_fight(g_widget, id, myRole, m_bag_item, m_bag_equip);
	m_dlg_fightfight->setWindowFlags(Qt::SubWindow);
	m_dlg_fightfight->move(g_widget->mapFromGlobal(g_widget->pos()) + QPoint(8, 30));
	m_dlg_fightfight->exec();

	delete m_dlg_fightfight;
	m_dlg_fightfight = nullptr;
}