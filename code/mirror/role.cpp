#include "role.h"
#include <QMessageBox>
#include <QFile>
#include <QMouseEvent>
#include "def_item_equip.h"

extern QVector<Info_Item> g_ItemList;
extern QVector<Info_equip> g_EquipList;
extern mapJobAdd g_mapJobAddSet;

QVector<quint64> g_lvExpList;			//升级经验设置表

role::role(RoleInfo *roleInfo, MapItem *bag_item, MapItem *storage_item)
: myTabFrame(NULL)
, myRole(roleInfo)
, m_bag_item(bag_item)
, m_storage_item(storage_item)
, m_tab_bagItem(bag_item)
, m_tab_storageItem(storage_item)
{
	ui.setupUi(this);
	m_dlg_equipInfo = nullptr;

	LoadExpSetting();
	LoadRole();

// 	ui.tabWidget_bag->addTab(&bag_equip, QStringLiteral("装备"));
 	ui.tabWidget_bag->addTab(&m_tab_bagItem, QStringLiteral("道具"));
// 	ui.tabWidget_bag->addTab(&storage_equip, QStringLiteral("装备仓库"));
 	ui.tabWidget_bag->addTab(&m_tab_storageItem, QStringLiteral("道具仓库"));

	myRole->equip[0] = { 300000, QUuid::createUuid(), 0 };
	myRole->equip[1] = { 301000, QUuid::createUuid(), 0 };
	DisplayEquip();
	DisplayRoleInfo();
	m_tab_storageItem.updateItemInfo(g_ItemList);

//  将控件保存到窗口中，方便后续直接采用循环处理
	EquipmentGrid.append(ui.lbl_equip_0);
	EquipmentGrid.append(ui.lbl_equip_1);
	EquipmentGrid.append(ui.lbl_equip_3);
	EquipmentGrid.append(ui.lbl_equip_4);
	EquipmentGrid.append(ui.lbl_equip_51);
	EquipmentGrid.append(ui.lbl_equip_52);
	EquipmentGrid.append(ui.lbl_equip_61);
	EquipmentGrid.append(ui.lbl_equip_62);
	EquipmentGrid.append(ui.lbl_equip_7);
	EquipmentGrid.append(ui.lbl_equip_8);
	EquipmentGrid.append(ui.lbl_equip_9);
	EquipmentGrid.append(ui.lbl_equip_10);

	//为装备栏控件安装事件过滤机制，使用QLabel控件可响应clicked()之类的事件。
	foreach (QLabel *lbl, EquipmentGrid)
	{
		lbl->installEventFilter(this);
	}
}

role::~role()
{

}

void role::updateRoleInfo(void)
{
	DisplayRoleInfo();
	
	m_tab_bagItem.updateItemInfo(g_ItemList);
}

void role::LoadRole()
{
	QFile file(SaveFileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		QString message = QStringLiteral("无法打开存档文件，存档可能已损坏或版本不匹配。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}

	qint32 ver;
	quint32 nTmp, nItemID, nItemCount;
	QDataStream out(file.readAll());
	out >> ver;
	if (ver != SaveFileVer)
	{
		QString message = QStringLiteral("无法打开存档文件，存档可能已损坏或版本不匹配。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}

	out >> myRole->name >> myRole->vocation >> myRole->gender;
	out >> myRole->coin >> myRole->gold >> myRole->reputation >> myRole->exp >> myRole->level;
	out >> myRole->strength >> myRole->wisdom >> myRole->spirit >> myRole->life >> myRole->agility >> myRole->potential;

	//加上身上装备---暂时认为没有装备
	memset(myRole->equip, 0, sizeof(EquitExtra) * MaxEquipCountForRole);

	//特别加载
	myRole->luck = 0;

	//加载道具背包信息
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> nItemID >> nItemCount;
		m_bag_item->insert(nItemID, nItemCount);
	}
	
	//加载道具仓库信息
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> nItemID >> nItemCount;
		m_storage_item->insert(nItemCount, nItemCount);
	}
	
	file.close();
	
	myRole->lvExp = g_lvExpList[myRole->level];
	myRole->intervel = qMax(quint32(1000), 1500 - myRole->agility);

	qint32 headNo = (myRole->vocation - 1) * 2 + myRole->gender;
	QString headImg = (":/role/Resources/role/role_") + QString::number(headNo) + ".png";
	ui.lbl_role_head->setPixmap(QPixmap(headImg));
	DisplayRoleInfo();
}
void role::DisplayRoleInfo(void)
{
	QString strTmp;
	qint32 nTmp;

	ui.edit_role_name->setText(myRole->name);
	ui.edit_role_vocation->setText(def_vocation[myRole->vocation]);
	ui.edit_role_coin->setText(QString::number(myRole->coin));
	ui.edit_role_reputation->setText(QString::number(myRole->reputation));
	ui.edit_role_level->setText(QString::number(myRole->level));
	ui.edit_role_luck->setText(QString::number(myRole->luck));

	ui.edit_role_strength->setText(QString::number(myRole->strength));
	ui.edit_role_wisdom->setText(QString::number(myRole->wisdom));
	ui.edit_role_spirit->setText(QString::number(myRole->spirit));
	ui.edit_role_life->setText(QString::number(myRole->life));
	ui.edit_role_agility->setText(QString::number(myRole->agility));
	ui.edit_role_potential->setText(QString::number(myRole->potential));

	strTmp = QString::number(myRole->exp) + "/" + QString::number(myRole->lvExp);
	ui.edit_role_exp->setText(strTmp);

	nTmp = qMax(quint32(1000), 1500 - myRole->agility);;
	ui.edit_role_interval->setText(QString::number(nTmp));

	//选择职业加成设置
	const QVector<Info_jobAdd> &vecJobAdd = g_mapJobAddSet[myRole->vocation];
	const Info_jobAdd &jobAdd = vecJobAdd[myRole->level - 1];

	myRole->dc1 = jobAdd.dc1 + equip_add.dc1 + myRole->strength / 10;
	myRole->dc2 = jobAdd.dc2 + equip_add.dc2 + myRole->strength / 5;
	if (myRole->dc2 < myRole->dc1)
	{
		myRole->dc2 = myRole->dc1;			//确保上限 >= 下限
	}
	ui.edit_role_dc->setText(QString::number(myRole->dc1) + "-" + QString::number(myRole->dc2));

	myRole->mc1 = jobAdd.mc1 + equip_add.mc1 + myRole->wisdom / 10;
	myRole->mc2 = jobAdd.mc2 + equip_add.mc2 + myRole->wisdom / 5;
	if (myRole->mc2 < myRole->mc1)
	{
		myRole->mc2 = myRole->mc1;
	}
	ui.edit_role_mc->setText(QString::number(myRole->mc1) + "-" + QString::number(myRole->mc2));

	myRole->sc1 = jobAdd.sc1 + equip_add.sc1 + myRole->spirit / 10;
	myRole->sc2 = jobAdd.sc2 + equip_add.sc2 + myRole->spirit / 5;
	if (myRole->sc2 < myRole->sc1)
	{
		myRole->sc2 = myRole->sc1;
	}
	ui.edit_role_sc->setText(QString::number(myRole->sc1) + "-" + QString::number(myRole->sc2));

	myRole->ac1 = jobAdd.ac1 + equip_add.ac1 + myRole->strength / 13;
	myRole->ac2 = jobAdd.ac2 + equip_add.ac2 + myRole->strength / 7;
	if (myRole->ac2 < myRole->ac1)
	{
		myRole->ac2 = myRole->ac1;
	}
	ui.edit_role_ac->setText(QString::number(myRole->ac1) + "-" + QString::number(myRole->ac2));

	myRole->mac1 = jobAdd.mac1 + equip_add.mac1 + myRole->wisdom / 15 + myRole->spirit / 14;
	myRole->mac2 = jobAdd.mac2 + equip_add.mac2 + myRole->wisdom / 8 + myRole->spirit / 7;
	if (myRole->mac2 < myRole->mac1)
	{
		myRole->mac2 = myRole->mac1;
	}
	ui.edit_role_mac->setText(QString::number(myRole->mac1) + "-" + QString::number(myRole->mac2));

	myRole->hp = jobAdd.hp + myRole->life * 25;
	ui.edit_role_hp->setText(QString::number(myRole->hp));

	myRole->mp = jobAdd.mp + myRole->life * 15;
	ui.edit_role_mp->setText(QString::number(myRole->mp));

	if (myRole->potential <= 0)
	{
		ui.btn_role_strength->setDisabled(true);
		ui.btn_role_wisdom->setDisabled(true);
		ui.btn_role_spirit->setDisabled(true);
		ui.btn_role_life->setDisabled(true);
		ui.btn_role_agility->setDisabled(true);
	}
	else
	{
		ui.btn_role_strength->setDisabled(false);
		ui.btn_role_wisdom->setDisabled(false);
		ui.btn_role_spirit->setDisabled(false);
		ui.btn_role_life->setDisabled(false);
		ui.btn_role_agility->setDisabled(false);
	}

	if (myRole->exp < myRole->lvExp)
	{
		ui.btn_role_lvUp->setDisabled(true);
	}
	else
	{
		ui.btn_role_lvUp->setDisabled(false);
	}

	if (myRole->level >= MaxLv)
	{
		ui.btn_role_lvUp->setDisabled(true);
	}
}
void role::Add_EquipAddPara(const Info_equip &equip)
{
	equip_add.ac1 += equip.ac1;
	equip_add.ac2 += equip.ac2;
	equip_add.mac1 += equip.mac1;
	equip_add.mac2 += equip.mac2;
	equip_add.dc1 += equip.dc1;
	equip_add.dc2 += equip.dc2;
	equip_add.mc1 += equip.mc1;
	equip_add.mc2 += equip.mc2;
	equip_add.sc1 += equip.sc1;
	equip_add.sc2 += equip.sc2;
}
void role::Sub_EquipAddPara(const Info_equip &equip)
{
	equip_add.ac1 -= equip.ac1;
	equip_add.ac2 -= equip.ac2;
	equip_add.mac1 -= equip.mac1;
	equip_add.mac2 -= equip.mac2;
	equip_add.dc1 -= equip.dc1;
	equip_add.dc2 -= equip.dc2;
	equip_add.mc1 -= equip.mc1;
	equip_add.mc2 -= equip.mc2;
	equip_add.sc1 -= equip.sc1;
	equip_add.sc2 -= equip.sc2;
}
void role::DisplayEquip()
{
	memset(&equip_add, 0, sizeof(Info_equip));

	//一次遍历，获取所有装备信息。
	for (qint32 j = 0; j < g_EquipList.size(); j++)
	{
		if (myRole->equip[0].ID == g_EquipList[j].ID)
		{
			ui.lbl_equip_0->setPixmap(g_EquipList[j].icon);
			Add_EquipAddPara(g_EquipList[j]);
		}
		if (myRole->equip[1].ID == g_EquipList[j].ID)
		{
			ui.lbl_equip_1->setPixmap(g_EquipList[j].icon);
			Add_EquipAddPara(g_EquipList[j]);
		}
		if (myRole->equip[2].ID == g_EquipList[j].ID)
		{
			ui.lbl_equip_3->setPixmap(g_EquipList[j].icon);
			Add_EquipAddPara(g_EquipList[j]);
		}
		if (myRole->equip[3].ID == g_EquipList[j].ID)
		{
			ui.lbl_equip_4->setPixmap(g_EquipList[j].icon);
			Add_EquipAddPara(g_EquipList[j]);
		}
		if (myRole->equip[4].ID == g_EquipList[j].ID || myRole->equip[5].ID == g_EquipList[j].ID)
		{
			if (myRole->equip[4].ID == g_EquipList[j].ID)
			{
				ui.lbl_equip_51->setPixmap(g_EquipList[j].icon);
				Add_EquipAddPara(g_EquipList[j]);
			}
			else
			{
				ui.lbl_equip_52->setPixmap(g_EquipList[j].icon);
				Add_EquipAddPara(g_EquipList[j]);
			}			
		}	
		if (myRole->equip[6].ID == g_EquipList[j].ID || myRole->equip[7].ID == g_EquipList[j].ID)
		{
			if (myRole->equip[6].ID == g_EquipList[j].ID)
			{
				ui.lbl_equip_61->setPixmap(g_EquipList[j].icon);
				Add_EquipAddPara(g_EquipList[j]);
			}
			else
			{
				ui.lbl_equip_62->setPixmap(g_EquipList[j].icon);
				Add_EquipAddPara(g_EquipList[j]);
			}	
		}		
		if (myRole->equip[8].ID == g_EquipList[j].ID)
		{
			ui.lbl_equip_7->setPixmap(g_EquipList[j].icon);
			Add_EquipAddPara(g_EquipList[j]);
		}
		if (myRole->equip[9].ID == g_EquipList[j].ID)
		{
			ui.lbl_equip_8->setPixmap(g_EquipList[j].icon);
			Add_EquipAddPara(g_EquipList[j]);
		}
		if (myRole->equip[10].ID == g_EquipList[j].ID)
		{
			ui.lbl_equip_9->setPixmap(g_EquipList[j].icon);
			Add_EquipAddPara(g_EquipList[j]);
		}
		if (myRole->equip[11].ID == g_EquipList[j].ID)
		{
			ui.lbl_equip_10->setPixmap(g_EquipList[j].icon);
			Add_EquipAddPara(g_EquipList[j]);
		}
	}
}
/*
bool role::CreateRole()
{
	QString name(QStringLiteral("mirror传奇"));

	myRole->reputation = myRole->exp = 0;
	myRole->strength = myRole->wisdom = myRole->spirit = myRole->life = myRole->agility = myRole->potential = 0;

	myRole->vocation = 1;
	myRole->level = 1;
	myRole->coin = 20000;
	myRole->gold = 1000;
#ifdef _DEBUG
	myRole->exp = 9000000;
#endif

	
	QFile file(db_role);
	if (!file.open(QIODevice::WriteOnly))
	{
		return false;
	}

	QDataStream out(&file);
	out << SaveFileVer;
	//基本信息
	out << name;
	out << myRole->vocation << myRole->coin << myRole->gold << myRole->reputation << myRole->exp << myRole->level;
//	out << myRole->hp << myRole->mp << myRole->dc1 << myRole->dc2 << myRole->mc1 << myRole->mc2 << myRole->sc1 << myRole->sc2;
//	out << myRole->ac1 << myRole->ac2 << myRole->mac1 << myRole->mac2 << myRole->luck;
	out << myRole->strength << myRole->wisdom << myRole->spirit << myRole->life << myRole->agility << myRole->potential;

	//道具背包，道具仓库皆为空。
	quint32 bag_item_size, store_item_size;
	bag_item_size = store_item_size = 0;
	out << bag_item_size << store_item_size;
	file.close();
	return true;
}
*/
void role::LoadExpSetting()
{
	QFile file("lvExpSet.db");
	if (!file.open(QIODevice::ReadOnly))
	{
		QString message = QStringLiteral("加载失败，请重新运行游戏。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}
	quint64 nTmp;
	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> nTmp;
		g_lvExpList.append(nTmp);
	}
}

void role::on_btn_mirror_save_clicked()
{
	qint32 nTmp;

	QFile file(SaveFileName);
	if (!file.open(QIODevice::WriteOnly))
	{
		QString message = QStringLiteral("无法保存，存档可能已损坏或不存在。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);
	}

	QDataStream out(&file);
	out << SaveFileVer;

	//保存基本信息
	out << myRole->name << myRole->vocation << myRole->gender;
	out << myRole->coin << myRole->gold << myRole->reputation << myRole->exp << myRole->level;
	out << myRole->strength << myRole->wisdom << myRole->spirit << myRole->life << myRole->agility << myRole->potential;

	//保存道具背包信息
	nTmp = m_bag_item->size();
	out << nTmp;
	
	for (MapItem::iterator iter = m_bag_item->begin(); iter != m_bag_item->end(); iter++)
	{
		out << iter.key() << iter.value();
	}

	//保存道具仓库信息
	out << nTmp;
	nTmp = m_storage_item->size();
	for (MapItem::iterator iter = m_storage_item->begin(); iter != m_storage_item->end(); iter++)
	{
		out << iter.key() << iter.value();
	}
	file.close();
}
void role::on_btn_role_strength_clicked()
{
	--myRole->potential;
	++myRole->strength;
	DisplayRoleInfo();
}
void role::on_btn_role_wisdom_clicked()
{
	--myRole->potential;
	++myRole->wisdom;
	DisplayRoleInfo();

}
void role::on_btn_role_spirit_clicked()
{
	--myRole->potential;
	++myRole->spirit;
	DisplayRoleInfo();
}
void role::on_btn_role_life_clicked()
{
	--myRole->potential;
	++myRole->life;
	DisplayRoleInfo();
}
void role::on_btn_role_agility_clicked()
{
	--myRole->potential;
	++myRole->agility;
	DisplayRoleInfo();
}
void role::on_btn_role_lvUp_clicked()
{
	myRole->exp -= myRole->lvExp;
	myRole->lvExp = g_lvExpList[myRole->level];
	++myRole->level;
	myRole->potential += 5;

	if (myRole->level >= MaxLv)
	{
		ui.btn_role_lvUp->setDisabled(true);
	}

	DisplayRoleInfo();
}
const Info_equip * role::FineEquip(quint32 id)
{
	foreach(const Info_equip &equip, g_EquipList)
	{
		if (equip.ID == id)
		{
			return &equip;
		}
	}
	return NULL;
}
void role::DisplayEquipInfo(QPoint pos, const Info_equip &equip)
{
	if (m_dlg_equipInfo == NULL)
	{
		m_dlg_equipInfo = new EquipInfo();
		m_dlg_equipInfo->setWindowFlags(Qt::WindowStaysOnTopHint);
	}
	
	m_dlg_equipInfo->updateInfo(pos, equip, *myRole);
	m_dlg_equipInfo->show();
}

bool role::eventFilter(QObject *obj, QEvent *ev)
{
	if (ev->type() == QEvent::MouseButtonRelease)
	{	
		QMouseEvent *mouseEvent = (QMouseEvent *)(ev);
		if (mouseEvent->button() == Qt::MouseButton::LeftButton)
		{	//左键显示装备详细信息
			for (quint32 i = 0; i < EquipmentGrid.size(); i++)
			{
				if (obj == EquipmentGrid[i])
				{
					const Info_equip *equip = FineEquip(myRole->equip[i].ID);
					if (equip != NULL)
					{					
						DisplayEquipInfo(mouseEvent->globalPos(), *equip);
						return  true;
					}
				}
			}
		}
		else if (mouseEvent->button() == Qt::MouseButton::RightButton)
		{
			//右键取下装备
			for (quint32 i = 0; i < EquipmentGrid.size(); i++)
			{
				if (obj == EquipmentGrid[i])
				{
					const Info_equip *equip = FineEquip(myRole->equip[i].ID);
					if (equip != NULL)
					{
						Sub_EquipAddPara(*equip);
						EquipmentGrid[i]->setPixmap(QPixmap(""));
						updateRoleInfo();
						return  true;
					}
				}
			}
		}
	}
	// pass the event on to the parent class
	return QWidget::eventFilter(obj, ev);		
}