#include "dlg_detail.h"
#include <QTextEdit>

Dlg_Detail::Dlg_Detail(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	hide();
}

Dlg_Detail::~Dlg_Detail()
{

}

void Dlg_Detail::on_btn_quit_clicked()
{
	hide();
}

QString GenerateEquipAttributeString(quint32 A2, quint32 extra, QString AttributeName)
{
	QString strTmp;
	//根据是否是极品确定字体颜色
	if (extra > 0)
		strTmp = QStringLiteral("`<font color = green>");
	else
		strTmp = QStringLiteral("`<font color = white>");

	//首先显示加成后的属性。
	strTmp += AttributeName + QStringLiteral("+") + QString::number(A2 + extra);

	//再显示极品属性
	if (extra > 0)
	{
		strTmp += QStringLiteral(" (") + QString::number(extra) + QStringLiteral(")");
	}
	strTmp += QStringLiteral("</font>");

	return strTmp;
}

QString GenerateEquipAttributeString(quint32 A1, quint32 A2, quint32 extra, QString AttributeName)
{
	QString strTmp;
	//根据是否是极品确定字体颜色
	if (extra > 0)
		strTmp = QStringLiteral("`<font color = green>");
	else
		strTmp = QStringLiteral("`<font color = white>");

	//首先显示加成后的属性。
	strTmp += AttributeName + QStringLiteral(" ") + QString::number(A1) + QStringLiteral("-") + QString::number(A2 + extra);

	//再显示极品属性
	if (extra > 0)
	{
		strTmp += QStringLiteral(" (") + QString::number(extra) + QStringLiteral(")");
	}
	strTmp += QStringLiteral("</font>");

	return strTmp;
}

void Dlg_Detail::DisplayEquipInfo(QPoint pos, const Info_basic_equip *BasicInfo, const Info_Equip *Equip, const RoleInfo *roleInfo)
{
	bool bSatisfy;
	QString strTmp;
	qint32 lineCount = 0;

	//如果是衣服类装备，则需要额外判断一下角色性别是否与装备需要性别相符合。
	strTmp = QStringLiteral("`<font color = yellow>");
	int Type = (BasicInfo->ID - g_itemID_start_equip) / 1000;
	if (Type == g_equipType_clothes_m || Type == g_equipType_clothes_f)
	{
		//性别不符，不可穿戴。
		//此次判断的简要解释： gender取值(male:1 female:2) Type取值(male:2 female:3)，故减1判相等即可
		if (roleInfo->gender != (Type - 1))
			strTmp = QStringLiteral("`<font color = red>");
	}
	strTmp += BasicInfo->name + QStringLiteral("</font>");
	ui.edit_display->setText(strTmp);

	ui.edit_display->append(QStringLiteral("`<font color = white>重量:1 持久:99/99</font>"));

	switch (Equip->extraAmount)
	{
	case 0:
		strTmp = QStringLiteral("`<font color = white>品质：白色</font>"); break;
	case 1:
	case 2:
		strTmp = QStringLiteral("`<font color = green>品质：绿色</font>"); break;
	case 3:
	case 4:
		strTmp = QStringLiteral("`<font color = blue>品质：蓝色</font>"); break;
	case 5:
	case 6:
		strTmp = QStringLiteral("`<font color = magenta>品质：紫色</font>"); break;
	case 7:
	case 8:
		strTmp = QStringLiteral("`<font color = #FEAB0D>品质：橙色</font>"); break;
	default:
		strTmp = QStringLiteral("`<font color = gray>品质：未知</font>"); 
		break;
	}
	ui.edit_display->append(strTmp);
	lineCount += 3;

	if (BasicInfo->luck + Equip->extra.luck > 0)
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->luck, Equip->extra.luck, QStringLiteral("幸运"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}

	if (BasicInfo->acc)
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->acc, 0, QStringLiteral("准确"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	if (BasicInfo->ag)
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->ag, 0, QStringLiteral("敏捷"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}

	if (BasicInfo->md > 0)
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->md * 10, 0, QStringLiteral("魔法躲避"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}

	if (BasicInfo->ac1 > 0 || (BasicInfo->ac2 + Equip->extra.ac > 0))
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->ac1, BasicInfo->ac2, Equip->extra.ac, QStringLiteral("防御"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}

	if (BasicInfo->mac1 > 0 || (BasicInfo->mac2 + Equip->extra.mac > 0))
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->mac1, BasicInfo->mac2, Equip->extra.mac, QStringLiteral("魔御"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	if (BasicInfo->dc1 > 0 || (BasicInfo->dc2 + Equip->extra.dc > 0))
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->dc1, BasicInfo->dc2, Equip->extra.dc, QStringLiteral("攻击"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	if (BasicInfo->mc1 > 0 || (BasicInfo->mc2 + Equip->extra.mc > 0))
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->mc1, BasicInfo->mc2, Equip->extra.mc, QStringLiteral("魔法"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	if (BasicInfo->sc1 > 0 || (BasicInfo->sc2 + Equip->extra.sc > 0))
	{
		strTmp = GenerateEquipAttributeString(BasicInfo->sc1, BasicInfo->sc2, Equip->extra.sc, QStringLiteral("道术"));
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	
	//查询角色当前属性是否符合佩带需要，如果不符合，则显示为红色，否则默认颜色。
	switch (BasicInfo->need)
	{
	case 0: bSatisfy = (roleInfo->level >= BasicInfo->needLvl); break;
	case 1: bSatisfy = (roleInfo->dc2 > BasicInfo->needLvl); break;
	case 2: bSatisfy = (roleInfo->mc2 > BasicInfo->needLvl); break;
	case 3: bSatisfy = (roleInfo->sc2 > BasicInfo->needLvl); break;
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
	this->resize(199, lineCount * 18);
}

void Dlg_Detail::DisplayItemInfo(QPoint pos, const Info_Item *item, quint32 no, quint32 role_voc, quint32 role_lvl)
{
	QString strTmp;
	qint32 lineCount = 10;
	qint32 nTmp;

	strTmp = QStringLiteral("`<font color = yellow>") + item->name + QStringLiteral("</font>");
	ui.edit_display->setText(strTmp);

	nTmp = (item->ID / 1000) % 100;
	QString def_ItemType[100] = { QStringLiteral("未知"), QStringLiteral("药品"), QStringLiteral("辅助"), QStringLiteral("经济"), QStringLiteral("属性调整") };
	def_ItemType[20] = QStringLiteral("书籍");
	def_ItemType[99] = QStringLiteral("杂项");
	ui.edit_display->append(QStringLiteral("`<font color = white>类型:") + def_ItemType[nTmp] + QStringLiteral("</font>"));

	//查询角色当前属性是否符合佩带需要，如果不符合，则显示为红色，否则默认颜色。
	bool bSatisfy = (role_lvl >= item->level);
	if (bSatisfy)
		strTmp = QStringLiteral("`<font color = white>等级:") + QString::number(item->level) + QStringLiteral("</font>");
	else
		strTmp = QStringLiteral("`<font color = red>等级:") + QString::number(item->level) + QStringLiteral("</font>");
	ui.edit_display->append(strTmp);

	if (item->vocation != 0)
	{
		if (item->vocation == role_voc)
			strTmp = QStringLiteral("`<font color = white>职业:") + def_vocation[item->vocation] +  QStringLiteral("</font>");
		else
			strTmp = QStringLiteral("`<font color = red>职业:") + def_vocation[item->vocation] + QStringLiteral("</font>");

		++lineCount;
		ui.edit_display->append(strTmp);
	}
	ui.edit_display->append(QStringLiteral("`<font color = white>重量:1</font>"));
	ui.edit_display->append(" ");//空行
	ui.edit_display->append(QStringLiteral("`<font color = white>单价:") + QString::number(item->coin) + QStringLiteral("</font>"));
	ui.edit_display->append(QStringLiteral("`<font color = white>数量:") + QString::number(no) + QStringLiteral("</font>"));
	ui.edit_display->append(" ");//空行
	ui.edit_display->append(QStringLiteral("`<font color = blue>") + item->descr + QStringLiteral("</font>"));

	this->move(pos);
	this->resize(199, lineCount * 18);
}
