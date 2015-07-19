#include "role.h"
#include <QMessageBox>
#include <QFile>
#include <QMouseEvent>
#include "dlg_count.h"
#include "def_takInfo.h"

extern RoleInfo_False g_falseRole;

extern QWidget *g_widget;

extern QVector<Info_basic_equip> g_EquipList;
extern QMap<itemID, Info_StateEquip> g_StateEquip;
extern QVector<Info_jobAdd> g_JobAddSet;
extern roleAddition g_roleAddition;
extern QVector<info_task> g_task_main_list;
Dlg_Detail *g_dlg_detail;

role::role(RoleInfo *roleInfo, MapRoleSkill *skill, MapItem *bag_item, MapItem *storage_item, ListEquip *bag_equip, ListEquip *storage_equip)
: QWidget(NULL)
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

#ifndef _DEBUG
	ui.lbl_test->setVisible(false);
	ui.edit_test_1->setVisible(false);
	ui.edit_test_2->setVisible(false);
	ui.edit_test_3->setVisible(false);
	ui.btn_test->setVisible(false);
#endif

	ui.btn_role_lvUp->setVisible(false);
	ui.checkBox_autoSave->setVisible(false);
	// 将控件保存到容器中，方便后续直接采用循环处理
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
	EquipmentGrid.append(ui.lbl_equip_11);
	EquipmentGrid.append(ui.lbl_equip_12);

	EquipPos[0] = ui.lbl_equip_0->pos();
	EquipPos[1] = ui.lbl_equip_1->pos();
	EquipPos[2] = ui.lbl_equip_3->pos();
	bShifePress = false;

	g_dlg_detail = new Dlg_Detail(this);
	g_dlg_detail->setWindowFlags(Qt::WindowStaysOnTopHint);

	Role_Lvl = (myRole->level >> 1) - 1;
	myRole->lvExp = g_JobAddSet[Role_Lvl].exp;
	if (myRole->gender == 1)
	{
		ui.lbl_role_backimg->setPixmap(QPixmap(":/ui/Resources/ui/1.png"));
	}
	else
	{
		ui.lbl_role_backimg->setPixmap(QPixmap(":/ui/Resources/ui/2.png"));
	}

	ui.stackedWidget->addWidget(&m_tab_equipBag);
	ui.stackedWidget->addWidget(&m_tab_itemBag);
	ui.stackedWidget->addWidget(&m_tab_equipStorage);
//	ui.stackedWidget->addWidget(&m_tab_storageItem);
	ui.stackedWidget->setCurrentIndex(0);

	DisplayEquip();
	DisplayRoleInfo();
	m_tab_equipBag.updateInfo();
	m_tab_itemBag.updateInfo();
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

void role::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Shift)
	{
		bShifePress = true;
	}
}
void role::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Shift)
	{
		bShifePress = false;
	}
}
inline void role::Broken32Bit(quint32 nSrc, quint8 &n1, quint8 &n2, quint8 &n3, quint8 &n4)
{
	n1 = nSrc >> 24;
	n2 = (nSrc >> 16) & 0xFF;
	n3 = (nSrc >> 8) & 0xFF;
	n4 = nSrc & 0xFF;
}

void role::DisplayRoleInfo(void)
{
	QString strTmp;
	qint32 nTmp;
	quint32 nTmp1, nTmp2;

	Role_Lvl = (myRole->level >> 1) - 1;
	quint64 role_exp = (myRole->exp >> 1) - 1;

	ui.edit_role_name->setText(myRole->name);
	ui.edit_role_vocation->setText(def_vocation[myRole->vocation]);
	ui.edit_role_coin->setText(QString::number((myRole->coin >> 1) - 1));
	ui.edit_role_reputation->setText(QString::number((myRole->reputation >> 1) -1 ));
	ui.edit_role_level->setText(QString::number(Role_Lvl));

	myRole->lvExp = g_JobAddSet[Role_Lvl].exp;
	strTmp = QString::number(role_exp) + "/" + QString::number(myRole->lvExp);
	ui.edit_role_exp->setText(strTmp);

	nTmp = qMax(1000, 1500 - myRole->equip_secret.speed);
	myRole->intervel_1 = (nTmp >> 8) & 0xff;
	myRole->intervel_2 = nTmp & 0xff;
	ui.edit_role_interval->setText(QString::number(nTmp));

	const Info_jobAdd &jobAdd = g_JobAddSet[Role_Lvl - 1];

	nTmp1 = jobAdd.dc1 + myRole->equip_add.dc1;
	nTmp2 = jobAdd.dc2 + myRole->equip_add.dc2;
	if (nTmp2 < nTmp1)
	{
		nTmp2 = nTmp1;			//确保上限 >= 下限
	}
	ui.edit_role_dc->setText(QString("%1-%2").arg(nTmp1).arg(nTmp2));

	nTmp1 = jobAdd.mc1 + myRole->equip_add.mc1;
	nTmp2 = jobAdd.mc2 + myRole->equip_add.mc2;
	if (nTmp2 < nTmp1)
	{
		nTmp2 = nTmp1;
	}
	ui.edit_role_mc->setText(QString("%1-%2").arg(nTmp1).arg(nTmp2));

	nTmp1 = jobAdd.sc1 + myRole->equip_add.sc1;
	nTmp2 = jobAdd.sc2 + myRole->equip_add.sc2;
	if (nTmp2 < nTmp1)
	{
		nTmp2 = nTmp1;
	}
	ui.edit_role_sc->setText(QString("%1-%2").arg(nTmp1).arg(nTmp2));

	nTmp1 = jobAdd.ac1 + myRole->equip_add.ac1;
	nTmp2 = jobAdd.ac2 + myRole->equip_add.ac2;
	if (nTmp2 < nTmp1)
	{
		nTmp2 = nTmp1;
	}
	ui.edit_role_ac->setText(QString("%1-%2").arg(nTmp1).arg(nTmp2));

	nTmp1 = jobAdd.mac1 + myRole->equip_add.mac1;
	nTmp2 = jobAdd.mac2 + myRole->equip_add.mac2;
	if (nTmp2 < nTmp1)
	{
		nTmp2 = nTmp1;
	}
	ui.edit_role_mac->setText(QString("%1-%2").arg(nTmp1).arg(nTmp2));

	nTmp1 = myRole->equip_add.ep;
	ui.edit_role_ep->setText(QString("%1 %").arg(nTmp1 * 0.01));

	nTmp1 = myRole->equip_add.ed;
	ui.edit_role_ed->setText(QString("%1").arg(nTmp1));

	g_falseRole.luck = ((myRole->equip_add.luck >> 4) & 0xFF) + myRole->equip_secret.luck;
	ui.edit_role_luck->setText(QString::number(g_falseRole.luck));

	myRole->acc = myRole->equip_add.acc & 0xFF;
	ui.edit_role_acc->setText(QString::number(myRole->acc));

	myRole->sacred = myRole->equip_add.sacred & 0xFF;

	ui.edit_role_agi->setText(QString::number(0));

	nTmp1 = jobAdd.hp + myRole->equip_secret.hp + Role_Lvl * myRole->equip_secret.ghp / 100;
	ui.edit_role_hp->setText(QString::number(nTmp1));

	nTmp1 = jobAdd.mp + +myRole->equip_secret.mp + Role_Lvl * myRole->equip_secret.gmp / 100;
	ui.edit_role_mp->setText(QString::number(nTmp1));
}
void role::EquipAddPara_Add(const Info_basic_equip &equip, const EquipExtra &extra, quint32 lvUp)
{
	quint32 nTmp, nSBV, nSGV;

	nTmp = equip.luck + extra.luck;
	myRole->equip_add.luck += nTmp << 4;
	myRole->equip_add.acc += equip.acc + extra.acc;
	myRole->equip_add.sacred += equip.sacred;
	myRole->equip_add.ep += equip.ep;
	myRole->equip_add.ed += equip.ed;
	myRole->equip_add.ac1 += equip.ac1;
	myRole->equip_add.ac2 += equip.ac2 + extra.ac;
	myRole->equip_add.mac1 += equip.mac1;
	myRole->equip_add.mac2 += equip.mac2 + extra.mac;
	myRole->equip_add.dc1 += equip.dc1;
	myRole->equip_add.dc2 += equip.dc2 + extra.dc;
	myRole->equip_add.mc1 += equip.mc1;
	myRole->equip_add.mc2 += equip.mc2 + extra.mc;
	myRole->equip_add.sc1 += equip.sc1;
	myRole->equip_add.sc2 += equip.sc2 + extra.sc;

	nSBV = equip.sbv;
	nSGV = equip.sgv;
	switch (equip.st)
	{
	case st_hp: myRole->equip_secret.hp += nSBV; myRole->equip_secret.ghp += nSGV;  break;
	case st_hpr: myRole->equip_secret.hpr += nSBV; myRole->equip_secret.ghpr += nSGV;  break;
	case st_hpd: myRole->equip_secret.hpd += nSBV; myRole->equip_secret.ghpd += nSGV;  break;
	case st_mp: myRole->equip_secret.mp += nSBV; myRole->equip_secret.gmp += nSGV;  break;
	case st_mpr: myRole->equip_secret.mpr += nSBV; myRole->equip_secret.gmpr += nSGV;  break;
	case st_mpd: myRole->equip_secret.mpd += nSBV; myRole->equip_secret.gmpd += nSGV;  break;
	case st_acc: myRole->equip_secret.acc += nSBV;  break;
	case st_macc: myRole->equip_secret.macc += nSBV;  break;
	case st_luck: myRole->equip_secret.luck += nSBV;  break;
	case st_speed: myRole->equip_secret.speed += nSBV;  break;
	default:
		break;
	}
}
void role::EquipAddPara_Sub(const Info_basic_equip &equip, const EquipExtra &extra, quint32 lvUp)
{
	quint32 nTmp, nSBV, nSGV;

	nTmp = equip.luck + extra.luck;
	myRole->equip_add.luck -= nTmp << 4;
	myRole->equip_add.acc -= equip.acc + extra.acc;
	myRole->equip_add.sacred -= equip.acc;
	myRole->equip_add.ep -= equip.ep;
	myRole->equip_add.ed -= equip.ed;
	myRole->equip_add.ac1 -= equip.ac1;
	myRole->equip_add.ac2 -= equip.ac2 + extra.ac;
	myRole->equip_add.mac1 -= equip.mac1;
	myRole->equip_add.mac2 -= equip.mac2 + extra.mac;
	myRole->equip_add.dc1 -= equip.dc1;
	myRole->equip_add.dc2 -= equip.dc2 + extra.dc;
	myRole->equip_add.mc1 -= equip.mc1;
	myRole->equip_add.mc2 -= equip.mc2 + extra.mc;
	myRole->equip_add.sc1 -= equip.sc1;
	myRole->equip_add.sc2 -= equip.sc2 + extra.sc;

	nSBV = equip.sbv;
	nSGV = equip.sgv;
	switch (equip.st)
	{
	case st_hp: myRole->equip_secret.hp -= nSBV; myRole->equip_secret.ghp -= nSGV;  break;
	case st_hpr: myRole->equip_secret.hpr -= nSBV; myRole->equip_secret.ghpr -= nSGV;  break;
	case st_hpd: myRole->equip_secret.hpd -= nSBV; myRole->equip_secret.ghpd -= nSGV;  break;
	case st_mp: myRole->equip_secret.mp -= nSBV; myRole->equip_secret.gmp -= nSGV;  break;
	case st_mpr: myRole->equip_secret.mpr -= nSBV; myRole->equip_secret.gmpr -= nSGV;  break;
	case st_mpd: myRole->equip_secret.mpd -= nSBV; myRole->equip_secret.gmpd -= nSGV;  break;
	case st_acc: myRole->equip_secret.acc -= nSBV;  break;
	case st_macc: myRole->equip_secret.macc -= nSBV;  break;
	case st_luck: myRole->equip_secret.luck -= nSBV;  break;
	case st_speed: myRole->equip_secret.speed -= nSBV;  break;
	default:
		break;
	}
}
void role::DisplayEquip()
{
	memset(&myRole->equip_add, 0, sizeof(Info_basic_equip));
	memset(&myRole->equip_secret, 0, sizeof(info_equip_secret));

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

	if (g_roleAddition.vecEquip[0].ID != 0)
	{
		const Info_StateEquip &stateEquip = g_StateEquip[g_roleAddition.vecEquip[0].ID];
		EquipmentGrid[0]->setPixmap(stateEquip.img);
		EquipmentGrid[0]->resize(stateEquip.img.size());
		EquipmentGrid[0]->move((EquipPos[0])-(QPoint(stateEquip.offset_x, stateEquip.offset_y)));
	}
	if (g_roleAddition.vecEquip[1].ID != 0)
	{
		EquipmentGrid[1]->setPixmap(g_StateEquip[g_roleAddition.vecEquip[1].ID].img);
	}
	if (g_roleAddition.vecEquip[2].ID != 0)
	{
		const Info_StateEquip &stateEquip = g_StateEquip[g_roleAddition.vecEquip[2].ID];
		EquipmentGrid[2]->setPixmap(stateEquip.img);
		EquipmentGrid[2]->resize(stateEquip.img.size());
		EquipmentGrid[2]->move((EquipPos[2]) - (QPoint(stateEquip.offset_x, stateEquip.offset_y)));
	}
}

void role::on_btn_mirror_save_clicked()
{
	emit mirrorSave();
}
// void role::on_btn_role_strength_clicked()
// {
// 	int n = 1;
// 	if (bShifePress) {
// 		if (g_roleAddition.potential < 10)	{
// 			n = g_roleAddition.potential;
// 		}	else	{
// 			n = 10;
// 		}
// 	}
// 	g_roleAddition.potential -= n;
// 	g_roleAddition.strength += n;
// 	DisplayRoleInfo();
// }
// void role::on_btn_role_wisdom_clicked()
// {
// 	int n = 1;
// 	if (bShifePress) {
// 		if (g_roleAddition.potential < 10)	{
// 			n = g_roleAddition.potential;
// 		}	else	{
// 			n = 10;
// 		}
// 	}
// 	g_roleAddition.potential -= n;
// 	g_roleAddition.wisdom += n;
// 	DisplayRoleInfo();
// }
// void role::on_btn_role_spirit_clicked()
// {
// 	int n = 1;
// 	if (bShifePress) {
// 		if (g_roleAddition.potential < 10)	{
// 			n = g_roleAddition.potential;
// 		}	else	{
// 			n = 10;
// 		}
// 	}
// 	g_roleAddition.potential -= n;
// 	g_roleAddition.spirit += n;
// 	DisplayRoleInfo();
// }
// void role::on_btn_role_life_clicked()
// {
// 	int n = 1;
// 	if (bShifePress) {
// 		if (g_roleAddition.potential < 10)	{
// 			n = g_roleAddition.potential;
// 		}	else	{
// 			n = 10;
// 		}
// 	}
// 	g_roleAddition.potential -= n;
// 	g_roleAddition.life += n;
// 	DisplayRoleInfo();
// }
//
// void role::on_btn_role_lvUp_clicked()
// {
// 	g_falseRole.exp -= myRole->lvExp;
// 	g_falseRole.level += 1;
// 
// 	myRole->exp -= myRole->lvExp << 1;
// 	myRole->level += 2;
// 	g_roleAddition.potential += 5;
// 	DisplayRoleInfo();
// }
void role::on_wearEquip(quint32 ID_for_new, quint32 index)
{
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
	case 12: locationA = 12; break;
	case 13: locationA = 13; break;
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

	//取下火焰戒指，删除技能
	if (EquipBasicInfo_old != nullptr && EquipBasicInfo_old->ID == 307019)
	{
		quint32 skillId = 220037;
		if (m_skill_study->contains(skillId))
		{
			m_skill_study->remove(skillId);
		}
		for (qint32 i = 0; i < myRole->skill.size(); i++)
		{
			if (myRole->skill.at(i).id == skillId)
			{
				myRole->skill.removeAt(i);
				break;
			}
		}
	}
	//戴上火焰戒指
	if (EquipBasicInfo_new->ID == 307019)
	{
		m_skill_study->insert(220037, 1);
	}
	
	if (locationA == 0 || locationA == 1 || locationA == 2)
	{
		const Info_StateEquip &stateEquip = g_StateEquip[g_roleAddition.vecEquip[locationA].ID];
		EquipmentGrid[locationA]->setPixmap(stateEquip.img);
		EquipmentGrid[locationA]->resize(stateEquip.img.size());
		EquipmentGrid[locationA]->move((EquipPos[locationA]) - (QPoint(stateEquip.offset_x, stateEquip.offset_y)));
	}
	else
	{
		EquipmentGrid[locationA]->setPixmap(EquipBasicInfo_new->icon);
	}
	updateRoleInfo();
	m_tab_equipBag.updateInfo();
}
void role::on_btn_test_clicked()
{
	qint32 locationA = ui.edit_test_1->text().toInt();
	qint32 posX = ui.edit_test_2->text().toInt();
	qint32 posY = ui.edit_test_3->text().toInt();
	EquipmentGrid[locationA]->move((EquipPos[locationA]) - (QPoint(posX, posY)));

}
void role::on_usedItem(quint32 ID)
{
	quint32 ItemCount = m_bag_item->value(ID);
	quint32 usedCount, nTmp;
	bool bTmp = false;
	roleSkill skill;
	QString strTmp;

	//弹出对话框询问使用数量。
	usedCount = 0;
	dlg_count *dlg = new dlg_count(this, QStringLiteral("使用量"), ItemCount);
	if (QDialog::Accepted == dlg->exec())
	{
		usedCount = dlg->getCount();
	}
	if (usedCount <= 0)
	{
		delete dlg;
		return;
	}
			
	//加成对应效果
	const Info_Item * itemItem = Item_Base::FindItem_Item(ID);
	nTmp = itemItem->value * usedCount;
	switch (itemItem->type)
	{
	case et_immediate_coin:		
		myRole->coin += nTmp << 1;
		ui.edit_role_coin->setText(QString::number((myRole->coin >> 1) -1));
		strTmp = QStringLiteral("金币增加：") + QString::number(nTmp);

		g_falseRole.coin += nTmp;
		break;
	case et_immediate_gold:
		myRole->gold += nTmp << 1;
		strTmp = QStringLiteral("元宝增加：") + QString::number(nTmp);

		g_falseRole.gold += nTmp;
		break;
	case et_immediate_reputation:
		myRole->reputation += nTmp << 1;
		ui.edit_role_reputation->setText(QString::number((myRole->reputation >> 1) -1));
		strTmp = QStringLiteral("声望增加：") + QString::number(nTmp);

		g_falseRole.reputation += nTmp;
		break;
	case et_skill:
		skill.id = itemItem->ID;
		skill.level = usedCount;
		
		if (m_skill_study->contains(skill.id))	{
			skill.level += m_skill_study->value(skill.id);
			bTmp = true;
		}

		if (skill.level > 3)	{
			usedCount -= skill.level - 3;
			skill.level = 3;	
		}

		if (usedCount > 0)
		{
			m_skill_study->insert(skill.id, skill.level);
			if (bTmp) {
				strTmp = QStringLiteral("技能《%1》等级提升。").arg(itemItem->name);
			} else	{
				strTmp = QStringLiteral("学会技能:《%1》").arg(itemItem->name);
			}
		}
		else
		{
			strTmp = QStringLiteral("技能《%1》等级已满。").arg(itemItem->name);
		}
		break;

	case et_ResetPotential:
		//重置角色属属点.
		//ResetPotential();
		//DisplayRoleInfo();
		//strTmp = QStringLiteral("因为感悟混沌的力量，你的属性点已重置！");
		strTmp = QStringLiteral("属性点系统已经无效！");
		break;
	case et_Level100:
		//99级筑基成100级.
		AdjustLevel(100);
		strTmp = QStringLiteral("你已经褪去凡根，成为一名强大的修士。成仙，已不再是梦想。");
	default:
		break;
	}

	if (!strTmp.isEmpty())
	{
		QMessageBox *msgBox = new QMessageBox;
		msgBox->setText(strTmp);
		msgBox->setWindowTitle(QStringLiteral("提示"));
		msgBox->addButton(QStringLiteral(" 我知道了 "), QMessageBox::AcceptRole);
		msgBox->exec();
	}

	ItemCount -= usedCount;
	if (ItemCount <= 0)
		m_bag_item->remove(ID);
	else
		m_bag_item->insert(ID, ItemCount);
	m_tab_itemBag.updateInfo();
}

void role::on_btn_bag_equip_clicked()
{
	g_dlg_detail->hide();
	ui.stackedWidget->setCurrentIndex(0);
	ui.label_bag_back->setPixmap(QPixmap(":/mirror/Resources/ui/r_0_2.png"));
}
void role::on_btn_bag_item_clicked()
{
	g_dlg_detail->hide();
	ui.stackedWidget->setCurrentIndex(1);
	ui.label_bag_back->setPixmap(QPixmap(":/mirror/Resources/ui/r_0_3.png"));
}
void role::on_btn_storage_equip_clicked()
{
	g_dlg_detail->hide();
	ui.stackedWidget->setCurrentIndex(2);
	ui.label_bag_back->setPixmap(QPixmap(":/mirror/Resources/ui/r_0_4.png"));
}
void role::DisplayEquipInfo(QPoint pos, const Info_basic_equip *BasicInfo, const Info_Equip *Equip)
{
	g_dlg_detail->DisplayEquipInfo(pos + QPoint(10, 10), BasicInfo, Equip, myRole);
	g_dlg_detail->show();
}
// void role::ResetPotential()
// {
// 	quint32 nTmp;
// 	nTmp = g_roleAddition.strength + g_roleAddition.wisdom + g_roleAddition.spirit + g_roleAddition.life + g_roleAddition.agility + g_roleAddition.potential;
// 	g_roleAddition.strength = g_roleAddition.wisdom = g_roleAddition.spirit = g_roleAddition.life = g_roleAddition.agility = 0;
// 	g_roleAddition.potential = nTmp;
// }

void role::AdjustLevel(qint32 Lvl)
{
	g_falseRole.exp = 0;
	g_falseRole.level = Lvl;

	myRole->exp = (g_falseRole.exp + 1 ) << 1;;
	myRole->level = (Lvl + 1) << 1;
// 	g_roleAddition.potential = (Lvl - 1) * 5;
// 	g_roleAddition.strength = 0;
// 	g_roleAddition.wisdom = 0;
// 	g_roleAddition.spirit = 0;
// 	g_roleAddition.life = 0;
// 	g_roleAddition.agility = 0;
	DisplayRoleInfo();
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

							if (EquipBasicInfo_old->ID == 307019)
							{
								quint32 skillId = 220037;
								if (m_skill_study->contains(skillId))
								{
									m_skill_study->remove(skillId);
								}
								for (qint32 i = 0; i < myRole->skill.size(); i++)
								{
									if (myRole->skill.at(i).id == skillId)
									{
										myRole->skill.removeAt(i);
										break;
									}
								}
							}
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