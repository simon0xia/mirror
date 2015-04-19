#include "mirror.h"
#include <QMessageBox>
#include <QFile>

QWidget *g_widget;
QVector<Info_Item> g_ItemList;					//游戏道具列表
QVector<Info_Distribute> g_MonsterDistribute;	//怪物分布列表。
QVector<MonsterInfo> g_MonsterNormal_List;		//普通怪物列表。
QVector<MonsterInfo> g_MonsterBoss_list;		//BOSS怪列表。
mapJobAdd g_mapJobAddSet;						//职业加成设定

mirror::mirror(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	g_widget = this;

	if (!LoadJobSet())
	{
		QString message = QString::fromLocal8Bit("加载职业设定失败，请重新运行游戏。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}

	if (!LoadItemList("./item_item.db")  || !LoadEquipList(""))
	{
		QString message = QString::fromLocal8Bit("加载道具失败，请重新运行游戏。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}
	if (!LoadMonster() || !LoadBoss() || !LoadDistribute())
	{
		QString message = QString::fromLocal8Bit("加载怪物失败，请重新运行游戏。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}
//	GiveSomeItem();

	m_tab_fight = new fight(&roleInfo, &m_bag_item);
	ui.tabWidget_main->addTab(m_tab_fight, QString::fromLocal8Bit("战斗"));

	m_tab_role = new role(&roleInfo, &m_bag_item, &m_storage_item);
	ui.tabWidget_main->addTab(m_tab_role, QString::fromLocal8Bit("角色"));

	m_tab_city = new city(&roleInfo, &m_bag_item);
	ui.tabWidget_main->addTab(m_tab_city, QString::fromLocal8Bit("城市"));

#ifdef _DEBUG
	{
		ui.tabWidget_main->setCurrentIndex(2);
	}
#else
	{
		ui.tabWidget_main->setCurrentIndex(1);
	}
#endif
			
	connect(ui.tabWidget_main, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}

mirror::~mirror()
{
	delete m_tab_fight;
	delete m_tab_role;
	delete m_tab_city;
}

void mirror::tabChanged(int index)
{
	QWidget *tab = ui.tabWidget_main->currentWidget();
	myTabFrame *frame = (myTabFrame *)tab;
	if (frame != NULL)
	{
		frame->updateRoleInfo();
	}
}

bool mirror::LoadJobSet()
{
	QFile file("jobSet.db");
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	Info_jobAdd job;
	QVector<Info_jobAdd> vec;
	quint32 count;

	quint32 vocation = 0;
	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		++vocation;
		vec.clear();

		out >> count;
		while (count--)
		{
			out >> job.level >> job.hp >> job.mp >> job.dc1 >> job.dc2 >> job.mc1 >> job.mc2
				>> job.sc1 >> job.sc2 >> job.ac1 >> job.ac2 >> job.mac1 >> job.mac2;

			vec.append(job);
		}

		g_mapJobAddSet[vocation] = vec;
	}

	file.close();
	return true;
}

bool mirror::LoadItemList(const QString &dbName)
{
	QFile file(dbName);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	Info_Item item;
	quint32 type;
	QImage img;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> item.ID >> item.name >> img >> item.sale >> item.level >> item.coin >> item.gold;
		out >> type >> item.value >> item.descr >> item.msg;
		
		item.icon = QPixmap::fromImage(img);
		item.type = static_cast<EffectType>(type);

		g_ItemList.append(item);
	}

	file.close();
	return true;
}
bool mirror::LoadEquipList(const QString &dbName)
{
	return true;
}

void mirror::GiveSomeItem()
{
	quint32 nCount = 6;
	quint32 nBagArr[] = { 0, 1, 4, 7, 9, 13 };
	quint32 nBagItemCount[] = { 100, 500, 300, 300, 1000, 20000 };

	quint32 nStorageArr[] = { 14, 15, 16, 21, 22, 30 };
	quint32 nStorageItemCount[] = { 200, 200, 200, 300, 300, 20 };
	
	
	quint32 ID;

	for (quint32 i = 0; i < nCount; i++)
	{
		ID = g_ItemList[nBagArr[i]].ID;
		m_bag_item[ID] = nBagItemCount[i];

		ID = g_ItemList[nStorageArr[i]].ID;
		m_storage_item[ID] = nStorageItemCount[i];
	}
}

bool mirror::LoadDistribute()
{
	QString db_distribute = "distribute.db";
	QFile file(db_distribute);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	quint32 id;
	QDataStream out(file.readAll());
	QVector<quint32> vec1, vec2;
	Info_Distribute distribute;
	while (!out.atEnd())
	{
		out >> distribute.mapID >> distribute.normal >> distribute.boss;
		g_MonsterDistribute.append(distribute);
	}

	file.close();
	return true;
}

bool mirror::LoadMonster()
{
	QString db_monster = "Monster_normal1.db";
	QFile file(db_monster);
	if (!file.open(QIODevice::ReadOnly))
	{
		false;
	}

	MonsterInfo mon;
	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> mon.ID >> mon.name >> mon.Head >> mon.level >> mon.exp >> mon.hp >> mon.mp;
		out >> mon.DC1 >> mon.DC2 >> mon.MC1 >> mon.MC2 >> mon.AC >> mon.MAC >> mon.interval;
		g_MonsterNormal_List.append(mon);
		
	}

	file.close();
	return true;
}

bool mirror::LoadBoss()
{
	QString db_boss = "Monster_boss1.db";
	QFile file(db_boss);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	MonsterInfo mon;
	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> mon.ID >> mon.name >> mon.Head >> mon.level >> mon.exp >> mon.hp >> mon.mp;
		out >> mon.DC1 >> mon.DC2 >> mon.MC1 >> mon.MC2 >> mon.AC >> mon.MAC >> mon.interval;
		g_MonsterBoss_list.append(mon);		
	}

	file.close();
	return true;
}