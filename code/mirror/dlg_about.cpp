#include "dlg_about.h"

dlg_about::dlg_about(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.edit_display->setStyleSheet("QTextEdit{ background:transparent} ");
	
	ui.edit_display->append(QStringLiteral("监  制: 蜗牛"));
	ui.edit_display->append(QStringLiteral("\n策  划: 蜗牛"));
	ui.edit_display->append(QStringLiteral("\n程  序: 蜗牛"));
	ui.edit_display->append(QStringLiteral("\n美  术: Asia 米饭"));
	ui.edit_display->append(QStringLiteral("\n音  效: 引号"));
	ui.edit_display->append(QStringLiteral("\n剧情设计: --"));
	ui.edit_display->append(QStringLiteral("\n任务设计: --"));
	ui.edit_display->append(QStringLiteral("\n关卡设计: 蜗牛"));
	ui.edit_display->append(QStringLiteral("\n新手指导: m歪歪m"));
	ui.edit_display->append(QStringLiteral("\n论坛管理: m歪歪m"));
	ui.edit_display->append(QStringLiteral("\n测    试: 蜗牛 引号 西瓜先森爱夏天°"));
	ui.edit_display->append(QStringLiteral("\n特别感谢(排名不分先后): "));
	ui.edit_display->append(QStringLiteral("Daniel \t灰色白衣 \t七叔 \tOnly once \t神经病有所好转"));
	ui.edit_display->append(QStringLiteral("小小佩恩 \tShaKespeare \t翎若 \t莴笋"));
}

dlg_about::~dlg_about()
{

}
