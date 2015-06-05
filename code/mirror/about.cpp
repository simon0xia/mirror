#include "about.h"

about::about(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.edit_display->setStyleSheet("QTextEdit{ background:transparent} ");

	resize(QSize(450, 500));
	
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
	ui.edit_display->append(QStringLiteral("\n论坛管理: m歪歪m"));
	ui.edit_display->append(QStringLiteral("\n测    试: 蜗牛 引号"));
	ui.edit_display->append(QStringLiteral("\n特别感谢(排名不分先后): "));
	ui.edit_display->append(QStringLiteral("Daniel \t灰色白衣 \t七叔 \tOnly once \t神经病有所好转"));
	ui.edit_display->append(QStringLiteral("米饭 \t小小佩恩 \tShaKespeare \t翎若"));
}

about::~about()
{

}
