#include "role.h"
#include <QMessageBox>
#include <QFile>
#include <QMouseEvent>
#include "dlg_count.h"
#include "role_skill.h"
#include "def_takInfo.h"

extern QWidget *g_widget;

extern QVector<Info_Item> g_ItemList;
extern QVector<Info_basic_equip> g_EquipList;
extern mapJobAdd g_mapJobAddSet;
extern QVector<quint64> g_lvExpList;
extern roleAddition g_roleAddition;
extern QVector<info_task> g_task_main_list;
Dlg_Detail *m_dlg_detail;

role::role(RoleInfo *roleInfo, VecRoleSkill *skill, MapItem *bag_item, MapItem *storage_item, ListEquip *bag_equip, ListEquip *storage_equip)
: myTabFrame(NULL)
, myRole(roleInfo)
, m_skill_study(skill)
, m_bag_item(bag_item)
, m_storage_item(storage_item)
, m_bag_equip(bag_equip)
, m_storage_equip(storage_equip)
, m_tab_itemBag(bag_item,roleInfo)
, m_tab_equipBag(roleInfo, bag_equip, storage_equip)
, m_tab_equipStorage(roleInfo, bag_equip, storage_equip)
{
	ui.setupUi(this);
	// 将控件保存到窗口中，方便后续直接采用循环处理
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

	ui.btn_task->setVisible(false);

	m_dlg_detail = new Dlg_Detail(this);
	m_dlg_detail->setWindowFlags(Qt::WindowStaysOnTopHint);

	qint32 headNo = ((myRole->vocation - 1) * 2 + myRole->gender) * 10;
	QString headImg = (":/role/Resources/role/") + QString::number(headNo) + ".png";
	ui.lbl_role_head->setPixmap(QPixmap(headImg));

 	ui.tabWidget_bag->addTab(&m_tab_equipBag, QStringLiteral("装备"));
 	ui.tabWidget_bag->addTab(&m_tab_itemBag, QStringLiteral("道具"));
 	ui.tabWidget_bag->addTab(&m_tab_equipStorage, QStringLiteral("装备仓库"));
// 	ui.tabWidget_bag->addTab(&m_tab_storageItem, QStringLiteral("道具仓库"));

	DisplayEquip();
	DisplayRoleInfo();
	m_tab_itemBag.updateInfo();
	m_tab_equipBag.updateInfo();
	m_tab_equipStorage.updateInfo();

	//为装备栏控件安装事件过滤机制，使得QLabel控件可响应clicked()之类的事件。
	foreach (QLabel *lbl, EquipmentGrid)
	{
		lbl->installEventFilter(this);
	}

	QObject::connect(&m_tab_equipBag, &Item_Base::UpdateEquipInfoSignals, this, &role::UpdateEquipInfo);
	QObject::connect(&m_tab_equipStorage, &Item_Base::UpdateEquipInfoSignals, this, &role::UpdateEquipInfo);
	QObject::connect(&m_tab_equipBag, &item_equipBag::wearEquip, this, &role::on_wearEquip);
	QObject::connect(&m_tab_equipBag, &Item_Base::UpdatePlayerInfoSignals, this, &role::updateRoleInfo);
	QObject::connect(&m_tab_itemBag, &item_itemBag::UsedItem, this, &role::on_usedItem);
	QObject::connect(&m_tab_itemBag, &Item_Base::UpdatePlayerInfoSignals, this, &role::updateRoleInfo);
}

role::~role()
{

}

void role::updateRoleInfo(void)
{
 	DisplayRoleInfo();
 	
 	m_tab_itemBag.updateInfo();
 	m_tab_equipBag.updateInfo();
}

void role::UpdateEquipInfo(void)
{
 	m_tab_equipBag.updateInfo();
 	m_tab_equipStorage.updateInfo();
}
void role::UpdateItemInfo(void)
{
	m_tab_itemBag.updateInfo();
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

	ui.edit_role_strength->setText(QString::number(g_roleAddition.strength));
	ui.edit_role_wisdom->setText(QString::number(g_roleAddition.wisdom));
	ui.edit_role_spirit->setText(QString::number(g_roleAddition.spirit));
	ui.edit_role_life->setText(QString::number(g_roleAddition.life));
	ui.edit_role_agility->setText(QString::number(g_roleAddition.agility));
	ui.edit_role_potential->setText(QString::number(g_roleAddition.potential));

	strTmp = QString::number(myRole->exp) + "/" + QString::number(myRole->lvExp);
	ui.edit_role_exp->setText(strTmp);

	nTmp = qMax(quint32(1000), 1500 - g_roleAddition.agility);;
	ui.edit_role_interval->setText(QString::number(nTmp));

	//选择职业加成设置
	const QVector<Info_jobAdd> &vecJobAdd = g_mapJobAddSet[myRole->vocation];
	const Info_jobAdd &jobAdd = vecJobAdd[myRole->level - 1];

	myRole->dc1 = jobAdd.dc1 + equip_add.dc1 + g_roleAddition.strength / 10;
	myRole->dc2 = jobAdd.dc2 + equip_add.dc2 + g_roleAddition.strength / 5;
	if (myRole->dc2 < myRole->dc1)
	{
		myRole->dc2 = myRole->dc1;			//确保上限 >= 下限
	}
	ui.edit_role_dc->setText(QString::number(myRole->dc1) + "-" + QString::number(myRole->dc2));

	myRole->mc1 = jobAdd.mc1 + equip_add.mc1 + g_roleAddition.wisdom / 10;
	myRole->mc2 = jobAdd.mc2 + equip_add.mc2 + g_roleAddition.wisdom / 5;
	if (myRole->mc2 < myRole->mc1)
	{
		myRole->mc2 = myRole->mc1;
	}
	ui.edit_role_mc->setText(QString::number(myRole->mc1) + "-" + QString::number(myRole->mc2));

	myRole->sc1 = jobAdd.sc1 + equip_add.sc1 + g_roleAddition.spirit / 10;
	myRole->sc2 = jobAdd.sc2 + equip_add.sc2 + g_roleAddition.spirit / 5;
	if (myRole->sc2 < myRole->sc1)
	{
		myRole->sc2 = myRole->sc1;
	}
	ui.edit_role_sc->setText(QString::number(myRole->sc1) + "-" + QString::number(myRole->sc2));

	myRole->ac1 = jobAdd.ac1 + equip_add.ac1 + g_roleAddition.strength / 13;
	myRole->ac2 = jobAdd.ac2 + equip_add.ac2 + g_roleAddition.strength / 7;
	if (myRole->ac2 < myRole->ac1)
	{
		myRole->ac2 = myRole->ac1;
	}
	ui.edit_role_ac->setText(QString::number(myRole->ac1) + "-" + QString::number(myRole->ac2));

	myRole->mac1 = jobAdd.mac1 + equip_add.mac1 + g_roleAddition.wisdom / 15 + g_roleAddition.spirit / 14;
	myRole->mac2 = jobAdd.mac2 + equip_add.mac2 + g_roleAddition.wisdom / 8 + g_roleAddition.spirit / 7;
	if (myRole->mac2 < myRole->mac1)
	{
		myRole->mac2 = myRole->mac1;
	}
	ui.edit_role_mac->setText(QString::number(myRole->mac1) + "-" + QString::number(myRole->mac2));

	myRole->luck = equip_add.luck;
	ui.edit_role_luck->setText(QString::number(myRole->luck));

	myRole->hp = jobAdd.hp + g_roleAddition.life * 25;
	ui.edit_role_hp->setText(QString::number(myRole->hp));

	myRole->mp = jobAdd.mp + g_roleAddition.life * 15;
	ui.edit_role_mp->setText(QString::number(myRole->mp));

	if (g_roleAddition.potential <= 0)
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
void role::EquipAddPara_Add(const Info_basic_equip &equip, const EquipExtra &extra, quint32 lvUp)
{
	equip_add.acc += equip.acc + extra.acc;
	equip_add.luck += equip.luck + extra.luck; 
	equip_add.ac1 += equip.ac1;
	equip_add.ac2 += equip.ac2 + extra.ac;
	equip_add.mac1 += equip.mac1;
	equip_add.mac2 += equip.mac2 + extra.mac;
	equip_add.dc1 += equip.dc1;
	equip_add.dc2 += equip.dc2 + extra.dc;
	equip_add.mc1 += equip.mc1;
	equip_add.mc2 += equip.mc2 + extra.mc;
	equip_add.sc1 += equip.sc1;
	equip_add.sc2 += equip.sc2 + extra.sc;
}
void role::EquipAddPara_Sub(const Info_basic_equip &equip, const EquipExtra &extra, quint32 lvUp)
{
	equip_add.acc -= equip.acc + extra.acc;
	equip_add.luck -= equip.luck + extra.luck;
	equip_add.ac1 -= equip.ac1;
	equip_add.ac2 -= equip.ac2 + extra.ac;
	equip_add.mac1 -= equip.mac1;
	equip_add.mac2 -= equip.mac2 + extra.mac;
	equip_add.dc1 -= equip.dc1;
	equip_add.dc2 -= equip.dc2 + extra.dc;
	equip_add.mc1 -= equip.mc1;
	equip_add.mc2 -= equip.mc2 + extra.mc;
	equip_add.sc1 -= equip.sc1;
	equip_add.sc2 -= equip.sc2 + extra.sc;
}
void role::DisplayEquip()
{
	memset(&equip_add, 0, sizeof(Info_basic_equip));

	for (quint32 i = 0; i < MaxEquipCountForRole; i++)
	{
		if (g_roleAddition.vecEquip[i].ID == 0)
		{
			continue;				//当前部位无装备
		}

		for (QVector<Info_basic_equip>::const_iterator iter = g_EquipList.begin(); iter != g_EquipList.end(); iter++)
		{
			if (g_roleAddition.vecEquip[i].ID == iter->ID)
			{
				EquipmentGrid[i]->setPixmap(iter->icon); 
				EquipAddPara_Add(*iter, g_roleAddition.vecEquip[i].extra, g_roleAddition.vecEquip[i].lvUp);
				break;
			}
		}
	}
}

void role::on_btn_mirror_save_clicked()
{
	emit mirrorSave();

/*	qint32 nTmp;

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
	
	//保存身上装备
	out.writeRawData((char *)myRole->vecEquip, sizeof(Info_Equip) * MaxEquipCountForRole);

	//保存玩家设定的挂机技能列表
	nTmp = myRole->skill.size();
	out << nTmp;
	for (VecRoleSkill::const_iterator iter = myRole->skill.begin(); iter != myRole->skill.end(); iter++)
	{
		out << iter->id << iter->level;
	}

	//保存道具背包信息
	nTmp = m_bag_item->size();
	out << nTmp;	
	for (MapItem::iterator iter = m_bag_item->begin(); iter != m_bag_item->end(); iter++)
	{
		out << iter.key() << iter.value();
	}

	//保存道具仓库信息
	nTmp = m_storage_item->size();
	out << nTmp;
	for (MapItem::iterator iter = m_storage_item->begin(); iter != m_storage_item->end(); iter++)
	{
		out << iter.key() << iter.value();
	}

	//保存装备背包信息
	nTmp = m_bag_equip->size();
	out << nTmp;
	for (ListEquip::iterator iter = m_bag_equip->begin(); iter != m_bag_equip->end(); iter++)
	{
		out.writeRawData((char *)&*iter, sizeof(Info_Equip));
	}

	//保存装备仓库信息
	nTmp = m_storage_equip->size();
	out << nTmp;
	for (ListEquip::iterator iter = m_storage_equip->begin(); iter != m_storage_equip->end(); iter++)
	{
		out.writeRawData((char *)&*iter, sizeof(Info_Equip));
	}

	nTmp = m_skill_study->size();
	out << nTmp;
	for (VecRoleSkill::const_iterator iter = m_skill_study->begin(); iter != m_skill_study->end(); iter++)
	{
		out << iter->id << iter->level;
	}

	file.close();*/
}
void role::on_btn_role_strength_clicked()
{
	--g_roleAddition.potential;
	++g_roleAddition.strength;
	DisplayRoleInfo();
}
void role::on_btn_role_wisdom_clicked()
{
	--g_roleAddition.potential;
	++g_roleAddition.wisdom;
	DisplayRoleInfo();

}
void role::on_btn_role_spirit_clicked()
{
	--g_roleAddition.potential;
	++g_roleAddition.spirit;
	DisplayRoleInfo();
}
void role::on_btn_role_life_clicked()
{
	--g_roleAddition.potential;
	++g_roleAddition.life;
	DisplayRoleInfo();
}
void role::on_btn_role_agility_clicked()
{
	--g_roleAddition.potential;
	++g_roleAddition.agility;
	DisplayRoleInfo();
}
void role::on_btn_role_lvUp_clicked()
{
	myRole->exp -= myRole->lvExp;
	myRole->lvExp = g_lvExpList[myRole->level];
	++myRole->level;
	g_roleAddition.potential += 5;

	if (myRole->level >= MaxLv)
	{
		ui.btn_role_lvUp->setDisabled(true);
	}

	DisplayRoleInfo();
}
void role::on_wearEquip(quint32 ID_for_new, quint32 index)
{
	UNREFERENCED_PARAMETER(ID_for_new);

	const Info_Equip &equip_new = m_bag_equip->at(index);
	const Info_basic_equip *EquipBasicInfo_new = Item_Base::GetEquipBasicInfo(equip_new.ID);
	
	//获取待佩带装备的类别
	int Type = (equip_new.ID - g_itemID_start_equip) / 1000;

	//根据类别映射到穿戴部位
	qint32 locationA, locationB;	
	locationA = locationB = -1;
	switch (Type)
	{
	case 1: locationA = 0; break;
	case 2: locationA = 1; break;
	case 3: locationA = 1; break;
	case 4: locationA = 2; break;
	case 5: locationA = 3; break;
	case 6: locationA = 4; locationB = 5; break;
	case 7: locationA = 6; locationB = 7; break;
	case 8: locationA = 8; break;
	case 9: locationA = 9; break;
	case 10: locationA = 10; break;
	case 11: locationA = 11; break;
	default:
		break;
	}

	//此装备可选装备左手/右手
	if (locationB != -1)
	{	//若左手有装备，右手为空，则装备在右手。否则装备在左手
		if (g_roleAddition.vecEquip[locationA].ID != 0 && g_roleAddition.vecEquip[locationB].ID == 0)
		{
			locationA = locationB;
		}
	}

	//扣除装备属性加成；将装备放入背包。
	const Info_basic_equip *EquipBasicInfo_old = Item_Base::GetEquipBasicInfo(g_roleAddition.vecEquip[locationA].ID);
	if (EquipBasicInfo_old != nullptr)
	{
		EquipAddPara_Sub(*EquipBasicInfo_old, g_roleAddition.vecEquip[locationA].extra, g_roleAddition.vecEquip[locationA].lvUp);
		m_bag_equip->append(g_roleAddition.vecEquip[locationA]);
	}
	//将背包装备从背包中取出来（删除）；更新装备属性加成，并显示相关信息
	EquipAddPara_Add(*EquipBasicInfo_new, equip_new.extra, equip_new.lvUp);
	g_roleAddition.vecEquip[locationA] = equip_new;
	m_bag_equip->removeAt(index);
	EquipmentGrid[locationA]->setPixmap(EquipBasicInfo_new->icon);
	updateRoleInfo();
	m_tab_equipBag.updateInfo();
}
void role::on_usedItem(quint32 ID)
{
	quint32 ItemCount = m_bag_item->value(ID);
	quint32 usedCount, nTmp;
	roleSkill skill;

	//弹出对话框询问使用数量。
	dlg_count *dlg = new dlg_count(this, QStringLiteral("使用量"), ItemCount);
	if (QDialog::Accepted != dlg->exec())
		return;

	usedCount = dlg->getCount();
	if (usedCount <= 0)
		return;
	delete dlg;
	
	ItemCount -= usedCount;
	if (ItemCount <= 0)
		m_bag_item->remove(ID);
	else
		m_bag_item->insert(ID, ItemCount);
	m_tab_itemBag.updateInfo();

	//加成对应效果
	const Info_Item * itemItem = Item_Base::FindItem_Item(ID);
	nTmp = itemItem->value * usedCount;
	switch (itemItem->type)
	{
	case et_immediate_coin:		
		myRole->coin += nTmp;
		ui.edit_role_coin->setText(QString::number(myRole->coin));
		break;
	case et_immediate_gold:
		myRole->gold += nTmp;
		break;
	case et_immediate_reputation:
		myRole->reputation += nTmp;
		ui.edit_role_reputation->setText(QString::number(myRole->reputation));
		break;
	case et_skill:
		skill.id = itemItem->ID;
		skill.level = (usedCount > 3 ? 3 : usedCount);
		for (VecRoleSkill::iterator iter = m_skill_study->begin(); iter != m_skill_study->end(); iter++)
		{
			if (iter->id == skill.id)
			{
				//已学会此技能，技能等级加1
				iter->level += usedCount;
				if (iter->level > 3)
				{
					iter->level = 3;
				}
				return;
			}
		}
		//新学此技能。
		m_skill_study->append(skill);
		return;
	default:
		break;
	}	
}
void role::on_btn_skill_clicked()
{
	role_skill *dlg_skill = new role_skill(this, m_skill_study, &myRole->skill);
	dlg_skill->setWindowFlags(Qt::Tool);
	//dlg_skill->move((pos()));
	dlg_skill->show();
}
void role::on_btn_task_clicked()
{
	const info_task &task = g_task_main_list[g_roleAddition.taskStep];
	
	QMessageBox *msgBox = new QMessageBox;
	QString strTmp = task.msg;
	msgBox->setText(strTmp);

	strTmp = QStringLiteral("任务奖励:");
	foreach(const itemID id, task.giveItem)
	{
		if (id > g_itemID_start_equip && id <= g_itemID_stop_equip)
		{
			const Info_basic_equip *EquipBasicInfo = Item_Base::GetEquipBasicInfo(id);
			if (EquipBasicInfo != nullptr)
			{
				strTmp += EquipBasicInfo->name + QStringLiteral(" 数量: ") + QString::number(task.giveCount);
			}
		}
		else if (id > g_itemID_start_item && id <= g_itemID_stop_item)
		{
			const Info_Item *itemInfo = Item_Base::FindItem_Item(id);
			if (itemInfo != nullptr)
			{
				strTmp += itemInfo->name + QStringLiteral(" 数量: ") + QString::number(task.giveCount);
			}			
		}	
		else
		{
			strTmp += QStringLiteral("读取奖励列表出错了");
		}
	}
	msgBox->setInformativeText(strTmp);
	
	QPushButton *YsBtn = msgBox->addButton(QStringLiteral(" 我已经带过来了 "), QMessageBox::AcceptRole);
	QPushButton *NoBtn = msgBox->addButton(QStringLiteral(" 我这就去 "), QMessageBox::RejectRole);
	msgBox->setDefaultButton(NoBtn);
	msgBox->exec();

	bool bTaskFinish = false;

	if (msgBox->clickedButton() == YsBtn)
	{
		if (task.requireItem > g_itemID_start_equip && task.requireItem <= g_itemID_stop_equip)
		{
			
		}
		else if (task.requireItem > g_itemID_start_item && task.requireItem <= g_itemID_stop_item)
		{

		}
		else
		{
			//nothing
		}
		foreach(const itemID id, task.giveItem)
		{
			if (id > g_itemID_start_equip && id <= g_itemID_stop_equip)
			{
				Info_Equip equip = { 0 };
				equip.ID = id;
				m_bag_equip->append(equip);
			}
			else if (id > g_itemID_start_item && id <= g_itemID_stop_item)
			{
				m_bag_item->insert(id, m_bag_item->value(id) + task.giveCount);
			}
			else
			{
				//nothing;
			}
		}
	}
}
void role::DisplayEquipInfo(QPoint pos, const Info_basic_equip *BasicInfo, const Info_Equip *Equip)
{
	m_dlg_detail->DisplayEquipInfo(pos + QPoint(10, 10), BasicInfo, Equip, myRole);
	m_dlg_detail->show();
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
					const Info_Equip &equip = g_roleAddition.vecEquip[i];
					if (equip.ID != 0)
					{				
						const Info_basic_equip *EquipBasicInfo = Item_Base::GetEquipBasicInfo(equip.ID);
						if (EquipBasicInfo != nullptr)
						{
							DisplayEquipInfo(this->mapFromGlobal(mouseEvent->globalPos()), EquipBasicInfo, &equip);
							return  true;
						}
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
					const Info_Equip &equip = g_roleAddition.vecEquip[i];
					if (equip.ID != 0)
					{
						const Info_basic_equip *EquipBasicInfo_old = Item_Base::GetEquipBasicInfo(equip.ID);
						if (EquipBasicInfo_old != nullptr)
						{
							EquipAddPara_Sub(*EquipBasicInfo_old, equip.extra, equip.lvUp);
							m_bag_equip->append(equip);
							g_roleAddition.vecEquip[i] = { 0 };

							m_tab_equipBag.updateInfo();
							EquipmentGrid[i]->setPixmap(QPixmap(""));
							updateRoleInfo();
							return  true;
						}
					}
				}
			}
		}
	}
	// pass the event on to the parent class
	return QWidget::eventFilter(obj, ev);		
}