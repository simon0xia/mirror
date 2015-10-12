#include "role.h"
#include <QMessageBox>
#include <QFile>
#include <QMouseEvent>
#include "dlg_count.h"
#include "def_takInfo.h"
#include "CommonComponents.h"

extern QWidget *g_widget;

extern QMap<skillID, Info_skill> g_skillList;
extern QVector<Info_basic_equip> g_EquipList;
extern QMap<itemID, Info_StateEquip> g_StateEquip;
extern QVector<Info_jobAdd> g_JobAddSet;
extern QVector<info_task> g_task_main_list;
Dlg_Detail *g_dlg_detail;

role::role(CPlayer *w_player)
: QWidget(NULL)
, player(w_player)
, m_tab_itemBag(w_player)
, m_tab_equipBag(w_player)
, m_tab_equipStorage(w_player)
{
	ui.setupUi(this);

#ifndef _DEBUG
	ui.lbl_test->setVisible(false);
	ui.edit_test_1->setVisible(false);
	ui.edit_test_2->setVisible(false);
	ui.edit_test_3->setVisible(false);
	ui.btn_test->setVisible(false);
#endif

	ui.btn_mirror_save->setVisible(false);
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

	g_dlg_detail = new Dlg_Detail(this, player);
	g_dlg_detail->setWindowFlags(Qt::WindowStaysOnTopHint);

	if (player->get_gender() == 1)
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
	ui.stackedWidget->setCurrentIndex(0);

	player->updateEquipInfo();
	player->updateParameter();
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

void role::DisplayRoleInfo(void)
{
	QString strTmp;
	qint32 nTmp;
	quint32 nTmp1, nTmp2;

	ui.edit_role_name->setText(player->get_name());
	ui.edit_role_vocation->setText(def_vocation[player->get_voc()]);
	ui.edit_role_coin->setText(QString::number(player->get_coin()));
	ui.edit_role_reputation->setText(QString::number(player->get_rep()));
	ui.edit_role_level->setText(QString::number(player->get_lv()));

	strTmp = QString::number(player->get_exp()) + "/" + QString::number(g_JobAddSet[player->get_lv()].exp);
	ui.edit_role_exp->setText(strTmp);

	ui.edit_role_hp->setText(QString::number(player->get_hp_max()));
	ui.edit_role_mp->setText(QString::number(player->get_mp_max()));
	ui.edit_role_interval->setText(QString::number(player->get_intervel()));

	ui.edit_role_immortal->setText(def_xiulian[player->get_lv() / 100]);

	ui.edit_role_dc->setText(QString("%1-%2").arg(player->get_dc1()).arg(player->get_dc2()));
	ui.edit_role_mc->setText(QString("%1-%2").arg(player->get_mc1()).arg(player->get_mc2()));
	ui.edit_role_sc->setText(QString("%1-%2").arg(player->get_sc1()).arg(player->get_sc2()));
	ui.edit_role_ac->setText(QString("%1-%2").arg(player->get_ac1()).arg(player->get_ac2()));
	ui.edit_role_mac->setText(QString("%1-%2").arg(player->get_mac1()).arg(player->get_mac2()));
	ui.edit_role_acc->setText(QString::number(player->get_acc()));
	ui.edit_role_agi->setText(QString::number(player->get_agi()));
	ui.edit_role_luck->setText(QString::number(player->get_luck()));
	ui.edit_role_scared->setText(QString::number(player->get_sacred()));	
}

void role::DisplayEquip()
{
	Info_Equip *onBodyEquip = player->get_onBodyEquip_point();

	for (quint32 i = 0; i < MaxEquipCountForRole; i++)
	{
		if (onBodyEquip[i].ID == 0)
		{
			continue;				//当前部位无装备
		}

		for (QVector<Info_basic_equip>::const_iterator iter = g_EquipList.begin(); iter != g_EquipList.end(); iter++)
		{
			if (onBodyEquip[i].ID == iter->ID)
			{
				EquipmentGrid[i]->setPixmap(iter->icon); 
				break;
			}
		}
	}

	if (onBodyEquip[0].ID != 0)
	{
		const Info_StateEquip &stateEquip = g_StateEquip.value(onBodyEquip[0].ID);
		EquipmentGrid[0]->setPixmap(stateEquip.img);
		EquipmentGrid[0]->resize(stateEquip.img.size());
		EquipmentGrid[0]->move((EquipPos[0])-(QPoint(stateEquip.offset_x, stateEquip.offset_y)));
	}
	if (onBodyEquip[1].ID != 0)
	{
		EquipmentGrid[1]->setPixmap(g_StateEquip.value(onBodyEquip[1].ID).img);
	}
	if (onBodyEquip[2].ID != 0)
	{
		const Info_StateEquip &stateEquip = g_StateEquip.value(onBodyEquip[2].ID);
		EquipmentGrid[2]->setPixmap(stateEquip.img);
		EquipmentGrid[2]->resize(stateEquip.img.size());
		EquipmentGrid[2]->move((EquipPos[2]) - (QPoint(stateEquip.offset_x, stateEquip.offset_y)));
	}
}

void role::on_btn_mirror_save_clicked()
{
//	emit mirrorSave();
}

void role::on_wearEquip(quint32 ID_for_new, quint32 index)
{
	itemID id = player->get_bag_equip()->at(index).ID;
	const Info_basic_equip *EquipBasicInfo_new = Item_Base::GetEquipBasicInfo(id);

	Info_Equip *onBodyEquip = player->get_onBodyEquip_point();

	qint32 locationA = player->wearEquip(index);
	if (0 > locationA)
	{
		//has error.
		return;
	}
	
	if (locationA == 0 || locationA == 1 || locationA == 2)
	{
		const Info_StateEquip &stateEquip = g_StateEquip[onBodyEquip[locationA].ID];
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
	quint32 ItemCount = player->get_bag_item()->value(ID);
	quint32 usedCount, nTmp;
	bool bTmp = false;
	roleSkill2 skill;
	QString strTmp;
	Info_Equip *equip;

	//弹出对话框询问使用数量。
	usedCount = 0;
	dlg_count *dlg = new dlg_count(this, QStringLiteral("使用量"), ItemCount);
	if (QDialog::Accepted == dlg->exec())
	{
		usedCount = dlg->getCount();
	}
	delete dlg;
	if (usedCount <= 0)
	{		
		return;
	}
			
	//加成对应效果
	const Info_Item * itemItem = Item_Base::FindItem_Item(ID);
	nTmp = itemItem->value * usedCount;
	switch (itemItem->type)
	{
	case et_immediate_coin:		
		player->add_coin(nTmp);
		ui.edit_role_coin->setText(QString::number(player->get_coin()));
		strTmp = QStringLiteral("金币增加：") + QString::number(nTmp);
		break;
	case et_immediate_gold:
		player->add_gold(nTmp);
		strTmp = QStringLiteral("元宝增加：") + QString::number(nTmp);
		break;
	case et_immediate_reputation:
		player->add_rep(nTmp);
		ui.edit_role_reputation->setText(QString::number(player->get_rep()));
		strTmp = QStringLiteral("声望增加：") + QString::number(nTmp);
		break;
	case et_skill:
		if (player->get_skill()->contains(itemItem->ID))	{
			skill = player->get_skill()->value(itemItem->ID);
			bTmp = true;
		}
		else
		{
			skill.id = itemItem->ID;
			skill.level = 0;
			skill.usdIndex = 0;
		}

		usedCount = qMin(usedCount, g_skillList.value(itemItem->ID).level - skill.level);
		if (usedCount > 0)
		{	
			skill.level += usedCount;
			player->get_skill()->insert(skill.id, skill);

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

	case et_lucky:
		equip = player->get_onBodyEquip_point();
		if (equip->extra.luck == itemItem->value - 1)
		{
			equip->extra.luck = itemItem->value;
			equip->extraAmount = equip->extra.acc + equip->extra.luck + equip->extra.ac + equip->extra.mac +
				equip->extra.dc + equip->extra.mc + equip->extra.sc;
			strTmp = QStringLiteral("赋予武器幸运值：%1").arg(itemItem->value);
		}
		else
		{
			strTmp = QStringLiteral("没事不要乱点，东西不要乱吃。");
		}
		player->updateEquipInfo();
		player->updateParameter();
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
		player->get_bag_item()->remove(ID);
	else
		player->get_bag_item()->insert(ID, ItemCount);
	m_tab_itemBag.updateInfo();
	DisplayRoleInfo();
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
	g_dlg_detail->DisplayEquipInfo(pos + QPoint(20,15), BasicInfo, Equip);
	g_dlg_detail->show();
}

void role::AdjustLevel(qint32 Lvl)
{
	player->set_exp(0);
	player->set_Lv(Lvl);
}

bool role::eventFilter(QObject *obj, QEvent *ev)
{
	if (ev->type() == QEvent::Enter)
	{
		for (quint32 i = 0; i < EquipmentGrid.size(); i++)
		{
 			if (obj == EquipmentGrid[i])
			{
				const Info_Equip &equip = player->get_onBodyEquip_point()[i];	
				const Info_basic_equip *EquipBasicInfo = Item_Base::GetEquipBasicInfo(equip.ID);
				if (EquipBasicInfo != nullptr)
				{
					g_dlg_detail->DisplayEquipInfo(this->mapFromGlobal(QCursor::pos()) + QPoint(20, 15), EquipBasicInfo, &equip);
					g_dlg_detail->show();
					return  true;
				}
			}
		}
	} 
	else if (ev->type() == QEvent::Leave)
	{
		g_dlg_detail->hide();
	}
	else if (ev->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent *mouseEvent = (QMouseEvent *)(ev);
		if (mouseEvent->button() == Qt::MouseButton::RightButton)
		{
			//右键取下装备
			for (quint32 i = 0; i < EquipmentGrid.size(); i++)
			{
				if (obj == EquipmentGrid[i])
				{
					const Info_Equip &equip = player->get_onBodyEquip_point()[i];
					const Info_basic_equip *EquipBasicInfo_old = Item_Base::GetEquipBasicInfo(equip.ID);
					if (EquipBasicInfo_old != nullptr)
					{
						player->takeoffEquip(i);
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