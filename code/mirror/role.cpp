#include "role.h"
#include <QMessageBox>
#include <QFile>
#include <QMouseEvent>
#include "Item_Base.h"

#include "dlg_detail.h"
#include "role_createEdt.h"

extern Dlg_Detail *g_dlg_detail;

extern QVector<QImage> g_dat_item;
//extern QMap<skillID, Info_SkillBasic> g_SkillBasic;
extern QMap<itemID, Info_basic_equip> g_EquipList;
extern QMap<itemID, Info_StateEquip> g_StateEquip;
extern QVector<QVector<Info_jobAdd>> g_JobAddSet;

role::role(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

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

	ChangedEmbodiment(0);

	//为装备栏控件安装事件过滤机制，使得QLabel控件可响应clicked()之类的事件。
	foreach (QLabel *lbl, EquipmentGrid)
	{
		lbl->installEventFilter(this);
	}
}

role::~role()
{

}

void role::on_btn_edt_body_clicked()
{
	ChangedEmbodiment(0);
}
void role::on_btn_edt_warrior_clicked()
{
	if (PlayerIns.get_edt_warrior().get_lv() <= 0)
	{
		role_createEdt ce(this, Voc_Warrior);
		if (QDialog::Rejected == ce.exec())
		{
			return;
		}
		emit UpdateCoin();
	}
	ChangedEmbodiment(1);
}
void role::on_btn_edt_magic_clicked()
{
	if (PlayerIns.get_edt_magic().get_lv() <= 0)
	{
		role_createEdt ce(this, Voc_Magic);
		if (QDialog::Rejected == ce.exec())
		{
			return;
		}
		emit UpdateCoin();
	}
	ChangedEmbodiment(2);
}
void role::on_btn_edt_taoshi_clicked()
{
	if (PlayerIns.get_edt_taoshi().get_lv() <= 0)
	{
		role_createEdt ce(this, Voc_Taoist);
		if (QDialog::Rejected == ce.exec())
		{
			return;
		}
		emit UpdateCoin();
	}
	ChangedEmbodiment(3);
}
void role::on_btn_changeFightState_clicked()
{
	if (PlayerIns.get_edt_Fight_index() != whichEdt)
	{
		PlayerIns.set_edt_fight(whichEdt);
		ui.btn_changeFightState->setText(QStringLiteral("已出战"));
	}
	else
	{
		PlayerIns.set_edt_fight(0);
		ui.btn_changeFightState->setText(QStringLiteral("休息中"));
	}	
}

void role::ChangedEmbodiment(int32_t which)
{
	PlayerIns.set_edt_current(which);
	Embodiment = &PlayerIns.get_edt_current();
	whichEdt = which;

	ui.btn_changeFightState->setVisible(which != 0);
	if (Embodiment->get_gender() == 1)
	{
		ui.lbl_role_backimg->setPixmap(QPixmap(":/ui/Resources/ui/1.png"));
	}
	else
	{
		ui.lbl_role_backimg->setPixmap(QPixmap(":/ui/Resources/ui/2.png"));
	}

	if (whichEdt != PlayerIns.get_edt_Fight_index()) {
		ui.btn_changeFightState->setText(QStringLiteral("休息中"));
	} else {
		ui.btn_changeFightState->setText(QStringLiteral("已出战"));
	}

	DisplayEquip();
	DisplayInfo();
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

void role::DisplayInfo(void)
{
	QString strTmp;

	ui.lbl_name->setText(Embodiment->get_name());
	ui.edit_role_vocation->setText(def_vocation[Embodiment->get_voc()]);
	ui.edit_role_level->setText(QString::number(Embodiment->get_lv()));
	
	int32_t lvExp = g_JobAddSet[Embodiment->get_voc() - 1].value(Embodiment->get_lv()).exp;
	strTmp = QString::number(Embodiment->get_exp()) + "/" + QString::number(lvExp);
	ui.edit_role_exp->setText(strTmp);

	ui.edit_role_dc->setText(QString("%1-%2").arg(Embodiment->get_dc1()).arg(Embodiment->get_dc2()));
	ui.edit_role_mc->setText(QString("%1-%2").arg(Embodiment->get_mc1()).arg(Embodiment->get_mc2()));
	ui.edit_role_sc->setText(QString("%1-%2").arg(Embodiment->get_sc1()).arg(Embodiment->get_sc2()));
	ui.edit_role_ac->setText(QString("%1").arg(Embodiment->get_ac()));
	ui.edit_role_mac->setText(QString("%1").arg(Embodiment->get_mac()));
	ui.edit_role_luck->setText(QString::number(Embodiment->get_luck()));

	ui.edit_role_hp->setText(QString::number(Embodiment->get_hp_max()));
	ui.edit_role_mp->setText(QString::number(Embodiment->get_mp_max()));
	ui.edit_role_hpr->setText(QString::number(Embodiment->get_rhp()));
	ui.edit_role_mpr->setText(QString::number(Embodiment->get_rmp()));
	ui.edit_role_interval->setText(QString::number(Embodiment->get_intervel()));
	ui.edit_hit->setText(QString::number(0));
	ui.edit_dodge->setText(QString::number(0));
	ui.edit_CritChance->setText(QString::number(0));
	ui.edit_CritDamage->setText(QString::number(0));
}

void role::DisplayEquip()
{
	const Info_Equip *onBodyEquip = Embodiment->get_onBodyEquip_point();

	int index = 0;
	for (; index < 3; index++)
	{
		if (!Embodiment->HasEquip(index))
		{
			EquipmentGrid[index]->setPixmap(QPixmap(""));
			continue;
		}
		itemID id = onBodyEquip[index].ID;
		if (g_StateEquip.contains(id))
		{
			const Info_StateEquip &stateEquip = g_StateEquip.value(onBodyEquip[index].ID);
			EquipmentGrid[index]->setPixmap(stateEquip.img);
			EquipmentGrid[index]->resize(stateEquip.img.size());
			EquipmentGrid[index]->move((EquipPos[index]) - (QPoint(stateEquip.offset_x, stateEquip.offset_y)));
		}
		else
		{
			EquipmentGrid[index]->setPixmap(QPixmap(""));
		}
	}
	for (; index < MaxEquipCountForRole; index++)
	{
		if (!Embodiment->HasEquip(index))
		{
			EquipmentGrid[index]->setPixmap(QPixmap(""));
			continue;
		}
		itemID id = onBodyEquip[index].ID;
		if (g_EquipList.contains(id))
		{
			qint32 imgId =g_EquipList.value(onBodyEquip[index].ID).icon;
			if (imgId >= g_dat_item.size() || imgId < 0) {
				imgId = 0;
			}
			EquipmentGrid[index]->setPixmap(QPixmap::fromImage(g_dat_item.at(imgId)));
		}
		else
		{
			EquipmentGrid[index]->setPixmap(QPixmap(""));
		}
	}
}

void role::DisplayEquip(qint32 locationA)
{
	const Info_Equip *onBodyEquip = Embodiment->get_onBodyEquip_point();
	itemID id = onBodyEquip[locationA].ID;
	if (locationA == 0 || locationA == 1 || locationA == 2)
	{
		const Info_StateEquip &stateEquip = g_StateEquip[id];
		EquipmentGrid[locationA]->setPixmap(stateEquip.img);
		EquipmentGrid[locationA]->resize(stateEquip.img.size());
		EquipmentGrid[locationA]->move((EquipPos[locationA]) - (QPoint(stateEquip.offset_x, stateEquip.offset_y)));
	}
	else
	{
		qint32 imgId = g_EquipList[id].icon;
		if (imgId >= g_dat_item.size() || imgId < 0) {
			imgId = 0;
		}
		EquipmentGrid[locationA]->setPixmap(QPixmap::fromImage(g_dat_item.at(imgId)));
	}
}

bool role::eventFilter(QObject *obj, QEvent *ev)
{
	if (ev->type() == QEvent::Enter)
	{
		for (qint32 i = 0; i < EquipmentGrid.size(); i++)
		{
 			if (obj == EquipmentGrid[i])
			{
				if (Embodiment->HasEquip(i))
				{
					const Info_Equip &equip = Embodiment->get_onBodyEquip_point()[i];
					const Info_basic_equip *EquipBasicInfo = GetEquipBasicInfo(equip.ID);
					if (EquipBasicInfo->ID > g_itemID_start_equip)
					{
						g_dlg_detail->DisplayEquipInfo(this->mapFromGlobal(QCursor::pos()) + QPoint(20, 15), EquipBasicInfo, &equip);
						g_dlg_detail->show();
					}
				}				
				return  true;
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
			for (qint32 i = 0; i < EquipmentGrid.size(); i++)
			{
				if (obj == EquipmentGrid[i])
				{
					//const Info_Equip &equip = Embodiment->get_onBodyEquip_point()[i];
					//const Info_basic_equip *EquipBasicInfo_old = GetEquipBasicInfo(equip.ID);
					//if (EquipBasicInfo_old != nullptr)
					if (Embodiment->HasEquip(i))
					{
						Embodiment->takeoffEquip(i);
						EquipmentGrid[i]->setPixmap(QPixmap(""));
						DisplayInfo();
						emit UpdateBag_BagEquip();
					}					
					return  true;
				}
			}
		}
	}
	
	// pass the event on to the parent class
	return QWidget::eventFilter(obj, ev);		
}