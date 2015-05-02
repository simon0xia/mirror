#include "mirror.h"
#include <QMessageBox>
#include <QFile>
#include "def_item_equip.h"

QWidget *g_widget;
QVector<quint64> g_lvExpList;					//升级经验设置表
QVector<Info_skill> g_skillList;				//技能设定
QVector<Info_Item> g_ItemList;					//游戏道具列表
QVector<Info_equip> g_EquipList;				//游戏装备列表
QVector<Info_Distribute> g_MonsterDistribute;	//怪物分布列表
QVector<MonsterInfo> g_MonsterNormal_List;		//普通怪物列表
QVector<MonsterInfo> g_MonsterBoss_list;		//BOSS怪列表
mapDrop	g_mapDropSet;							//怪物暴率设定
mapJobAdd g_mapJobAddSet;						//职业加成设定

mirror::mirror(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	g_widget = this;

	this->setWindowTitle(QStringLiteral("mirror传奇_beta_0.0.1"));

	if (!LoadExpSetting() || !LoadRole() || !LoadJobSet())
	{
		QString message = QStringLiteral("加载职业设定失败，请重新运行游戏。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}

	if (!LoadSkill() || !LoadItemList()  || !LoadEquipList())
	{
		QString message = QStringLiteral("加载技能、道具或装备失败，请重新运行游戏。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}
	if (!LoadMonster() || !LoadBoss() || !LoadDistribute() || !LoadDropSet())
	{
		QString message = QStringLiteral("加载怪物失败，请重新运行游戏。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}

	m_tab_fight = new fight(&roleInfo, &m_bag_item, &m_bag_equip);
	ui.tabWidget_main->addTab(m_tab_fight, QStringLiteral("战斗"));

	m_tab_role = new role(&roleInfo, &m_skill_study, &m_bag_item, &m_storage_item, &m_bag_equip, &m_storage_equip);
	ui.tabWidget_main->addTab(m_tab_role, QStringLiteral("角色"));

	m_tab_city = new city(&roleInfo, &m_bag_item);
	ui.tabWidget_main->addTab(m_tab_city, QStringLiteral("城市"));

#ifdef _DEBUG
	{
		ui.tabWidget_main->setCurrentIndex(1);
	}
#else
	{
		ui.tabWidget_main->setCurrentIndex(1);
	}
#endif
	bgAudioList = nullptr;
	bgAudio = nullptr;
	if (QFile::exists("./sound/b-2.mp3"))
	{
		bgAudioList = new QMediaPlaylist;
		bgAudioList->setPlaybackMode(QMediaPlaylist::Loop);
		bgAudio = new QMediaPlayer;
		bgAudio->setPlaylist(bgAudioList);
		bgAudio->setVolume(80);
		bgAudioList->addMedia(QUrl::fromLocalFile("./sound/b-2.mp3"));
		bgAudioList->setCurrentIndex(0);
		bgAudio->play();
	}
	
	connect(ui.tabWidget_main, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}

mirror::~mirror()
{
	delete m_tab_fight;
	delete m_tab_role;
	delete m_tab_city;

	bgAudio->stop();
	delete bgAudio;
	delete bgAudioList;
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
	QFile file("./db/jobSet.db");
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
bool mirror::LoadSkill()
{
	QFile file("./db/skill.db");
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	Info_skill skill;
	QImage img1,img2;
	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> skill.ID >> skill.name >> img1 >> img2 >> skill.level >> skill.times;
		out >> skill.damage[0] >> skill.damage[1] >> skill.damage[2] >> skill.buff >> skill.buff_time >> skill.descr;

		skill.icon1 = QPixmap::fromImage(img1);
		skill.icon2 = QPixmap::fromImage(img2);

		g_skillList.append(skill);
	}

	file.close();
	return true;
}
bool mirror::LoadItemList()
{
	QFile file("./db/item_item.db");
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
		out >> item.ID >> item.name >> img >> item.vocation >> item.level >> item.sale >> item.coin >> item.gold;
		out >> type >> item.value >> item.descr >> item.msg;
		
		item.icon = QPixmap::fromImage(img);
		item.type = static_cast<EffectType>(type);

		g_ItemList.append(item);
	}

	file.close();
	return true;
}
bool mirror::LoadEquipList()
{
	QFile file("./db/item_equip.db");
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	Info_equip equip;
	QImage img;
	quint32 type, nA,nB;
	QDataStream out(file.readAll());
	while (!out.atEnd())
	{		
		out >> equip.ID >> equip.name >> img >> equip.lv >> nA >> nB >> equip.mac1 >> equip.mac2;
		out >> equip.dc1 >> equip.dc2 >> equip.mc1 >> equip.mc2 >> equip.sc1 >> equip.sc2;
		out >> equip.need >> equip.needLvl >> equip.price >> equip.msg;

		equip.icon = QPixmap::fromImage(img);

		equip.luck = equip.acc = equip.ac1 = equip.ac2 = 0;
		type = QString::number(equip.ID).mid(1, 2).toUInt();
		if (type == 1)
		{
			equip.luck = nA;
			equip.acc = nB;
		}
		else
		{
			equip.ac1 = nA;
			equip.ac2 = nB;
		}

		g_EquipList.append(equip);
	}

	file.close();
	return true;
}

void mirror::GiveSomeItem()
{
	m_bag_equip.append(301001);
	m_bag_equip.append(301025);
	m_bag_equip.append(311003);
	m_bag_equip.append(302001);
	m_bag_equip.append(303005);
	m_bag_equip.append(304004);
	m_bag_equip.append(305013);
	m_bag_equip.append(307016);
	m_bag_equip.append(308022);
	m_bag_equip.append(310001);

	m_bag_item[201001] = 10;
	m_bag_item[201004] = 10;
	m_bag_item[201011] = 10;
	m_bag_item[201020] = 10;
	m_bag_item[202001] = 10;
	m_bag_item[202010] = 10;
	m_bag_item[203002] = 10;
	m_bag_item[203018] = 10;
}

bool mirror::LoadDistribute()
{
	QFile file("./db/distribute.db");
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
		out >> distribute.ID >> distribute.normal >> distribute.boss;
		g_MonsterDistribute.append(distribute);
	}

	file.close();
	return true;
}

bool mirror::LoadMonster()
{
	QFile file("./db/Monster_normal1.db");
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
	QFile file("./db/Monster_boss1.db");
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

bool mirror::LoadDropSet()
{
	QFile file("./db/drop.db");
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	quint32 monsterID, ListSize;
	Rational rRat;
	QList<Rational> listDrop;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> monsterID >> ListSize;

		listDrop.clear();
		while (ListSize--)
		{
			out >> rRat.ID >> rRat.den;
			listDrop.append(rRat);
		}
		g_mapDropSet[monsterID] = listDrop;
	}

	file.close();
	return true;
}

bool mirror::LoadExpSetting()
{
	QFile file("./db/lvExpSet.db");
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	quint64 nTmp;
	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> nTmp;
		g_lvExpList.append(nTmp);
	}
	return true;
}

bool mirror::LoadRole()
{
	QFile file(SaveFileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	qint32 ver;
	quint32 nTmp, nItemID, nItemCount;
	roleSkill skill;
	QDataStream out(file.readAll());
	out >> ver;
	if (ver != SaveFileVer)
	{
		QString message = QStringLiteral("无法打开存档文件，存档可能已损坏或版本不匹配。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}

	out >> roleInfo.name >> roleInfo.vocation >> roleInfo.gender;
	out >> roleInfo.coin >> roleInfo.gold >> roleInfo.reputation >> roleInfo.exp >> roleInfo.level;
	out >> roleInfo.strength >> roleInfo.wisdom >> roleInfo.spirit >> roleInfo.life >> roleInfo.agility >> roleInfo.potential;

	//加载身上装备
	for (qint32 i = 0; i < MaxEquipCountForRole; i++)
	{
		out >> roleInfo.equip[i];
	}

	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> skill.id >> skill.level;
		roleInfo.skill.append(skill);
	}

	//加载道具背包信息
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> nItemID >> nItemCount;
		m_bag_item.insert(nItemID, nItemCount);
	}

	//加载道具仓库信息
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> nItemID >> nItemCount;
		m_storage_item.insert(nItemCount, nItemCount);
	}

	//加载装备背包信息
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> nItemID;
		m_bag_equip.append(nItemID);
	}

	//加载装备仓库信息
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> nItemID;
		m_storage_equip.append(nItemID);
	}

	//加载技能
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> skill.id >> skill.level;
		m_skill_study.append(skill);
	}

	file.close();

	roleInfo.lvExp = g_lvExpList[roleInfo.level];
	roleInfo.intervel = qMax(quint32(1000), 1500 - roleInfo.agility);

	return true;
}