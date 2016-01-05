#include "fight_orginfo.h"
#include "MonsterDefine.h"

#include <QMap>


extern QMap<monsterID, MonsterInfo> g_MonsterInfo;

fight_OrgInfo::fight_OrgInfo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QFont f = ui.edit_display->font();
	QFontMetrics fm(f);
	lineSpacing = fm.lineSpacing();
}

fight_OrgInfo::~fight_OrgInfo()
{

}

void fight_OrgInfo::DisplayMonsterInfo(QPoint pos, monsterID id)
{
	QString strTmp;
	qint32 lineCount = 12;

	if (!g_MonsterInfo.contains(id))
	{
		id = g_MonsterInfo.firstKey();	
	}
	const MonsterInfo &info = g_MonsterInfo.value(id);

	ui.edit_display->setText(QStringLiteral("名字：%1").arg(info.name));
	ui.edit_display->append(QStringLiteral("等级：%1").arg(info.level));
	ui.edit_display->append(QStringLiteral("体力：%1").arg(info.hp));
	ui.edit_display->append(QStringLiteral("法力：%1").arg(info.mp));

	ui.edit_display->append("");
	ui.edit_display->append(QStringLiteral("攻击：%1-%2").arg(info.DC1).arg(info.DC2));
	ui.edit_display->append(QStringLiteral("魔法：%1-%2").arg(info.MC1).arg(info.MC2));
	ui.edit_display->append(QStringLiteral("物防：%1").arg(info.AC));
	ui.edit_display->append(QStringLiteral("魔防：%1").arg(info.MAC));
	ui.edit_display->append(QStringLiteral("攻击间隔：%1").arg(info.interval));

	ui.edit_display->append("");
	ui.edit_display->append(QStringLiteral("标准经验：%1").arg(info.exp));

	this->move(pos);
	this->resize(150, lineCount * lineSpacing + 12);
}