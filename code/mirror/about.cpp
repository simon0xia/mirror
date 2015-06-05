#include "about.h"

about::about(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.edit_display->setStyleSheet("QTextEdit{ background:transparent} ");

	ui.edit_display->append(QStringLiteral("出品人: 蜗牛"));
	ui.edit_display->append(QStringLiteral("\n制作人: 蜗牛"));
	ui.edit_display->append(QStringLiteral("\n策  划: 蜗牛"));
	ui.edit_display->append(QStringLiteral("\n程  序: 蜗牛"));
	ui.edit_display->append(QStringLiteral("\nUI设计: Asia"));
//	ui.edit_display->append(QStringLiteral("\n美  术: 蜗牛"));
	ui.edit_display->append(QStringLiteral("\n音  乐: 引号"));
	ui.edit_display->append(QStringLiteral("\n剧情设计: 莴笋"));
	ui.edit_display->append(QStringLiteral("\n任务设计: 莴笋 蜗牛"));
	ui.edit_display->append(QStringLiteral("\n关卡设计: 莴笋 蜗牛"));
	ui.edit_display->append(QStringLiteral("\n新手指导: m歪歪m"));
	ui.edit_display->append(QStringLiteral("\n测    试: 蜗牛 向阳的植物 引号"));
	ui.edit_display->append(QStringLiteral("\n特别感谢(排名不分先后): "));
	ui.edit_display->append(QStringLiteral("Daniel 七叔 神经病有所好转 Only once 米饭 小小佩恩 ShaKespeare 翎若"));
}

about::~about()
{

}
