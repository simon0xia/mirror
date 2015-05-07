#include "role.h"
#include <QMessageBox>
#include <QFile>
#include <QMouseEvent>
#include "dlg_count.h"
#include "role_skill.h"

extern QWidget *g_widget;

extern QVector<Info_Item> g_ItemList;
extern QVector<Info_equip> g_EquipList;
extern mapJobAdd g_mapJobAddSet;
extern QVector<quint64> g_lvExpList;

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

	myRole->luck = equip_add.luck;
	ui.edit_role_luck->setText(QString::number(myRole->luck));

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
	equip_add.acc += equip.acc;
	equip_add.luck += equip.luck;
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
	equip_add.acc -= equip.acc;
	equip_add.luck -= equip.luck;
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

	for (quint32 i = 0; i < MaxEquipCountForRole; i++)
	{
		if (myRole->equip[i] == 0)
		{
			continue;				//当前部位无装备
		}

		for (QVector<Info_equip>::const_iterator iter = g_EquipList.begin(); iter != g_EquipList.end(); iter++)
		{
			if (myRole->equip[i] == iter->ID)
			{
				EquipmentGrid[i]->setPixmap(iter->icon); 
				Add_EquipAddPara(*iter);
				break;
			}
		}
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
	//保存身上装备
	for (quint32 i = 0; i < MaxEquipCountForRole;i++)
	{
		out << myRole->equip[i];
	}

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
		out << *iter;
	}

	//保存装备仓库信息
	nTmp = m_storage_equip->size();
	out << nTmp;
	for (ListEquip::iterator iter = m_storage_equip->begin(); iter != m_storage_equip->end(); iter++)
	{
		out << *iter;
	}

	nTmp = m_skill_study->size();
	out << nTmp;
	for (VecRoleSkill::const_iterator iter = m_skill_study->begin(); iter != m_skill_study->end(); iter++)
	{
		out << iter->id << iter->level;
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
void role::on_wearEquip(quint32 ID_for_new, quint32 index)
{
	const Info_equip *equip_new = Item_Base::FindItem_Equip(ID_for_new);
	
	//获取待佩带装备的类别
	int Type = (ID_for_new % 100000) / 1000;

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
		if (myRole->equip[locationA] != 0 && myRole->equip[locationB] == 0)
		{
			locationA = locationB;
		}
	}

	//扣除装备属性加成；将装备放入背包。
	const Info_equip *equip = Item_Base::FindItem_Equip(myRole->equip[locationA]);
	if (equip != NULL)
	{
		Sub_EquipAddPara(*equip);
		m_bag_equip->append(equip->ID);
	}

	//将背包装备从背包中取出来（删除）；更新装备属性加成，并显示相关信息
	m_bag_equip->removeAt(index);
	Add_EquipAddPara(*equip_new);
	myRole->equip[locationA] = equip_new->ID;
	EquipmentGrid[locationA]->setPixmap(equip_new->icon);
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
void role::DisplayEquipInfo(QPoint pos, const Info_equip &equip)
{
	m_dlg_detail->DisplayEquipInfo(pos, &equip, myRole);
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
					const Info_equip *equip = Item_Base::FindItem_Equip(myRole->equip[i]);
					if (equip != NULL)
					{
						DisplayEquipInfo(this->mapFromGlobal(mouseEvent->globalPos()), *equip);
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
					const Info_equip *equip = Item_Base::FindItem_Equip(myRole->equip[i]);
					if (equip != NULL)
					{
						myRole->equip[i] = 0;
						Sub_EquipAddPara(*equip);
						m_bag_equip->append(equip->ID);
						m_tab_equipBag.updateInfo();
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