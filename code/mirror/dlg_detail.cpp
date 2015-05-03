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

void Dlg_Detail::DisplayEquipInfo(QPoint pos, const Info_equip *equip, const RoleInfo *roleInfo)
{
	bool bSatisfy;
	QString strTmp;
	qint32 lineCount = 0;

	//如果是衣服类装备，则需要额外判断一下角色性格是否与装备需要性别相符合。
	strTmp = QStringLiteral("`<font color = yellow>");
	//获取待佩带装备的类别
	int Type = (equip->ID % 100000) / 1000;
	if (Type == 2 || Type == 3)
	{
		//性别不符，不可穿戴。
		//此次判断的简要解释： gender取值(male:1 female:2) Type取值(male:2 female:3)，故减1判相等即可
		if (roleInfo->gender != (Type - 1))
			strTmp = QStringLiteral("`<font color = red>");
	}
	strTmp += equip->name + QStringLiteral("</font>");
	ui.edit_display->setText(strTmp);

	ui.edit_display->append(QStringLiteral(" 重量:1 持久:99/99"));
	lineCount += 2;

	if (equip->luck > 0)
	{
		ui.edit_display->append(QStringLiteral(" 幸运+") + QString::number(equip->luck));
		++lineCount;
	}
	if (equip->acc > 0)
	{
		ui.edit_display->append(QStringLiteral(" 准确+") + QString::number(equip->acc));
		++lineCount;
	}

	if (equip->ac1 > 0 || equip->ac2 > 0)
	{
		strTmp = QStringLiteral(" 防御") + QString::number(equip->ac1) + QStringLiteral("-") + QString::number(equip->ac2);
		ui.edit_display->append(strTmp);
		++lineCount;
	}

	if (equip->mac1 > 0 || equip->mac2 > 0)
	{
		strTmp = QStringLiteral(" 魔御") + QString::number(equip->mac1) + QStringLiteral("-") + QString::number(equip->mac2);
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	if (equip->dc1 > 0 || equip->dc2 > 0)
	{
		strTmp = QStringLiteral(" 攻击") + QString::number(equip->dc1) + QStringLiteral("-") + QString::number(equip->dc2);
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	if (equip->mc1 > 0 || equip->mc2 > 0)
	{
		strTmp = QStringLiteral(" 魔法") + QString::number(equip->mc1) + QStringLiteral("-") + QString::number(equip->mc2);
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	if (equip->sc1 > 0 || equip->sc2 > 0)
	{
		strTmp = QStringLiteral(" 道术") + QString::number(equip->sc1) + QStringLiteral("-") + QString::number(equip->sc2);
		ui.edit_display->append(strTmp);
		++lineCount;
	}
	
	//查询角色当前属性是否符合佩带需要，如果不符合，则显示为红色，否则默认颜色。
	switch (equip->need)
	{
	case 0: bSatisfy = (roleInfo->level >= equip->needLvl); break;
	case 1: bSatisfy = (roleInfo->dc2 > equip->needLvl); break;
	case 2: bSatisfy = (roleInfo->mc2 > equip->needLvl); break;
	case 3: bSatisfy = (roleInfo->sc2 > equip->needLvl); break;
	default:
		bSatisfy = false;
		break;
	}

	strTmp = "";
	if (!bSatisfy)
	{
		strTmp += QStringLiteral("`<font color = red>");
	}
	strTmp += equip_need_info[equip->need] + QString::number(equip->needLvl);
	if (!bSatisfy)
	{
		strTmp += QStringLiteral("</font>");
	}
	ui.edit_display->append(strTmp);

	ui.edit_display->append(QStringLiteral(" ******************************"));
	ui.edit_display->append(QStringLiteral("`<font color = blue>") + equip->msg + QStringLiteral("</font>"));
	lineCount += 3;
	lineCount++;

	this->move(pos);
	this->resize(199, lineCount * 18);
}

void Dlg_Detail::DisplayItemInfo(QPoint pos, const Info_Item *item, quint32 role_voc, quint32 role_lvl)
{
	QString strTmp;
	qint32 lineCount = 7;
	qint32 nTmp;

	strTmp = QStringLiteral("`<font color = yellow>") + item->name + QStringLiteral("</font>");
	ui.edit_display->setText(strTmp);

	nTmp = (item->ID / 1000) % 100;
	QString def_ItemType[21] = { QStringLiteral("无"), QStringLiteral("药品"), QStringLiteral("辅助"), QStringLiteral("经济") };
	def_ItemType[20] = QStringLiteral("技能书籍");
	ui.edit_display->append(QStringLiteral(" 道具类型:") + def_ItemType[nTmp]);

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
	ui.edit_display->append(QStringLiteral(" 重量:1"));
	ui.edit_display->append(QStringLiteral(" 价格:") + QString::number(item->coin));
	ui.edit_display->append(" ");//空行
	ui.edit_display->append(QStringLiteral("`<font color = blue>") + item->descr + QStringLiteral("</font>"));

	this->move(pos);
	this->resize(199, lineCount * 19);
}
