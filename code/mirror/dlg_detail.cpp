#include "dlg_detail.h"
#include <QTextEdit>

Dlg_Detail::Dlg_Detail(QWidget *parent, CPlayer *w_player)
	: QDialog(parent), player(w_player)
{
	ui.setupUi(this);
	hide();

	QFont f = ui.edit_display->font();
	QFontMetrics fm(f);
	lineSpacing = fm.lineSpacing();
}

Dlg_Detail::~Dlg_Detail()
{

}

void Dlg_Detail::on_btn_quit_clicked()
{
	hide();
}

QString GenerateEquipAttributeString(quint32 A2, const QString &AttributeName)
{
	QString strTmp;
	strTmp = QStringLiteral("`<font color = white>");

	//首先显示加成后的属性。
	strTmp += AttributeName + QStringLiteral(" +%1 </font>").arg(A2);

	return strTmp;
}

QString GenerateEquipAttributeString(quint32 A1, quint32 A2, const QString &AttributeName)
{
	QString strTmp;
	strTmp = QStringLiteral("`<font color = white>");

	//首先显示加成后的属性。
	strTmp += AttributeName + QStringLiteral(" %1-%2</font>").arg(A1).arg(A2);

	return strTmp;
}

void Dlg_Detail::DisplayEquipInfo(QPoint pos, const Info_basic_equip *BasicInfo, const Info_Equip *Equip)
{
	bool bSatisfy;
	QString strTmp;
	qint32 lineCount = 0;
	quint32 nTmp;

	//如果是衣服类装备，则需要额外判断一下角色性别是否与装备需要性别相符合。
	strTmp = QStringLiteral("`<font color = yellow>%1</font>").arg(BasicInfo->name);
	int Type = (BasicInfo->ID - g_itemID_start_equip) / 1000;
	if (Type == g_equipType_clothes_m || Type == g_equipType_clothes_f)
	{
		//性别不符，不可穿戴。
		//此次判断的简要解释： gender取值(male:1 female:2) Type取值(male:2 female:3)，故减1判相等即可
		if (player->get_gender() != (Type - 1))
			strTmp = QStringLiteral("`<font color = red>%1</font>").arg(BasicInfo->name);
	}
	ui.edit_display->setText(strTmp);

	ui.edit_display->append(QStringLiteral("`<font color = white>重量：1 持久:99/99</font>"));
	ui.edit_display->append(QStringLiteral("`<font color = white>等阶：%1</font>").arg(BasicInfo->lv));

	switch (Equip->extraAmount)
	{
	case 0:
		strTmp = QStringLiteral("`<font color = white>品质：普通</font>"); break;
	case 1:
		strTmp = QStringLiteral("`<font color = green>品质：精良</font>"); break;
	case 2:
		strTmp = QStringLiteral("`<font color = blue>品质：稀有</font>"); break;
	case 3:
		strTmp = QStringLiteral("`<font color = magenta>品质：卓越</font>"); break;
	case 4:
		strTmp = QStringLiteral("`<font color = #FEAB0D>品质：完美</font>"); break;		
	default:
		strTmp = QStringLiteral("`<font color = gray>品质：未知</font>"); 
		break;
	}
	ui.edit_display->append(strTmp);
	ui.edit_display->append("");
	ui.edit_display->append(QStringLiteral("<font color = yellow>基本属性</font>"));
	lineCount += 6;
	
	if (BasicInfo->luck > 0)
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->luck, QStringLiteral("幸运"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	if (BasicInfo->spd > 0)
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->spd, QStringLiteral("攻击速度"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}

	if (BasicInfo->hp > 0)
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->hp, QStringLiteral("生命"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	if (BasicInfo->mp > 0)
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->mp, QStringLiteral("法力"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	if (BasicInfo->ac > 0)
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->ac, QStringLiteral("防御"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}

	if (BasicInfo->mac > 0)
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->mac, QStringLiteral("魔御"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	if (BasicInfo->dc1 > 0 || (BasicInfo->dc2 > 0))
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->dc1, BasicInfo->dc2, QStringLiteral("攻击"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	if (BasicInfo->mc1 > 0 || (BasicInfo->mc2 > 0))
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->mc1, BasicInfo->mc2, QStringLiteral("魔法"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	if (BasicInfo->sc1 > 0 || (BasicInfo->sc2 > 0))
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->sc1, BasicInfo->sc2, QStringLiteral("道术"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}

	if (Equip->extraAmount > 0)
	{
		ui.edit_display->append("");
		ui.edit_display->append(QStringLiteral("<font color = yellow>极品属性</font>"));
		lineCount += 2;

		for (int i = 0; i < Equip->extraAmount; i++)
		{
			strTmp = "";
			const EquipExtra2 &extra = Equip->extra[i];
			switch (extra.eet)
			{
			case eet_fixed_hp: strTmp = QStringLiteral("生命 +%1").arg(extra.value); break;
			case eet_fixed_mp: strTmp = QStringLiteral("法力 +%1").arg(extra.value); break;
			case eet_fixed_hpr: strTmp = QStringLiteral("生命恢复 +%1").arg(extra.value); break;
			case eet_fixed_mpr: strTmp = QStringLiteral("法力恢复 +%1").arg(extra.value); break;
			case eet_fixed_dc: strTmp = QStringLiteral("攻击 +%1").arg(extra.value); break;
			case eet_fixed_mc: strTmp = QStringLiteral("魔法 +%1").arg(extra.value); break;
			case eet_fixed_sc: strTmp = QStringLiteral("道术 +%1").arg(extra.value); break;
			case eet_fixed_ac: strTmp = QStringLiteral("防御 +%1").arg(extra.value); break;
			case eet_fixed_mac: strTmp = QStringLiteral("魔御 +%1").arg(extra.value); break;
			case eet_fixed_spd: strTmp = QStringLiteral("攻击速度 +%1").arg(extra.value); break;
			case eet_fixed_luck:strTmp = QStringLiteral("幸运 +%1").arg(extra.value); break;
			case eet_percent_hp: strTmp = QStringLiteral("生命 +%1%").arg(extra.value); break;
			case eet_percent_mp: strTmp = QStringLiteral("法力 +%1%").arg(extra.value); break;
			case eet_percent_hpr: strTmp = QStringLiteral("生命恢复 +%1%").arg(extra.value); break;
			case eet_percent_mpr: strTmp = QStringLiteral("法力恢复 +%1%").arg(extra.value); break;
			case eet_percent_dc: strTmp = QStringLiteral("攻击 +%1%").arg(extra.value); break;
			case eet_percent_mc: strTmp = QStringLiteral("魔法 +%1%").arg(extra.value); break;
			case eet_percent_sc: strTmp = QStringLiteral("道术 +%1%").arg(extra.value); break;
			case eet_percent_ac: strTmp = QStringLiteral("防御 +%1%").arg(extra.value); break;
			case eet_percent_mac: strTmp = QStringLiteral("魔御 +%1%").arg(extra.value); break;
			default:
				break;
			}

			if (!strTmp.isEmpty())
			{
				ui.edit_display->append(QString("`<font color = white>%1</font>").arg(strTmp));
				lineCount ++;
			}
		}
	}
	
	//查询角色当前属性是否符合佩带需要，如果不符合，则显示为红色，否则默认颜色。
	switch (BasicInfo->need)
	{
	case 0: 
		bSatisfy = (player->get_lv() >= BasicInfo->needLvl);
		break;
	case 1: 
		bSatisfy = (player->get_dc2() >= BasicInfo->needLvl); 
		break;
	case 2: 
		bSatisfy = (player->get_mc2() >= BasicInfo->needLvl);
		break;
	case 3: 
		bSatisfy = (player->get_sc2() >= BasicInfo->needLvl);
		break;
	default:
		bSatisfy = false;
		break;
	}

	if (bSatisfy)
	{
		strTmp = QStringLiteral("`<font color = white>");
	}
	else
	{
		strTmp = QStringLiteral("`<font color = red>");
	}
	strTmp += equip_need_info[BasicInfo->need] + QString::number(BasicInfo->needLvl);
	strTmp += QStringLiteral("</font>");

	ui.edit_display->append(strTmp);
	lineCount+= 2;

	this->move(pos);
	this->resize(150, lineCount * lineSpacing + 12);
}

void Dlg_Detail::DisplayItemInfo(QPoint pos, const Info_Item *item, quint32 no)
{
	QString strTmp;
	qint32 lineCount = 11;
	qint32 nTmp;

	strTmp = QStringLiteral("`<font color = yellow>") + item->name + QStringLiteral("</font>");
	ui.edit_display->setText(strTmp);

	nTmp = (item->ID / 1000) % 100;
	QString def_ItemType[100] = { QStringLiteral("未知"), QStringLiteral("药品"), QStringLiteral("辅助"), QStringLiteral("经济"), QStringLiteral("属性调整") };
	def_ItemType[20] = QStringLiteral("书籍");
	def_ItemType[99] = QStringLiteral("杂项");
	ui.edit_display->append(QStringLiteral("`<font color = white>类型:%1</font>").arg(def_ItemType[nTmp]));

	//查询角色当前属性是否符合佩带需要，如果不符合，则显示为红色，否则默认颜色。
	bool bSatisfy = (player->get_lv() >= item->level);
	if (bSatisfy)
		strTmp = QStringLiteral("`<font color = white>等级:%1</font>").arg(item->level);
	else
		strTmp = QStringLiteral("`<font color = red>等级:%1</font>").arg(item->level);
	ui.edit_display->append(strTmp);

	if (item->vocation != 0)
	{
		if (item->vocation == player->get_voc())
			strTmp = QStringLiteral("`<font color = white>职业:%1</font>").arg(def_vocation[item->vocation]);
		else
			strTmp = QStringLiteral("`<font color = red>职业:%1</font>").arg(def_vocation[item->vocation]);

		++lineCount;
		ui.edit_display->append(strTmp);
	}
	ui.edit_display->append(QStringLiteral("`<font color = white>重量:1</font>"));
	ui.edit_display->append(" ");//空行
	ui.edit_display->append(QStringLiteral("`<font color = white>单价:%1</font>").arg(item->coin));
	ui.edit_display->append(QStringLiteral("`<font color = white>数量:%1</font>").arg(no));
	ui.edit_display->append(" ");//空行
	ui.edit_display->append(QStringLiteral("`<font color = cyan>%1</font>").arg(item->descr));

	this->move(pos);
	this->resize(199, lineCount * lineSpacing + 12);
}
