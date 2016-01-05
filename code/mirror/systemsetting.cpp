#include "systemsetting.h"
#include <QSettings>

SystemSetting::SystemSetting(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ReadSystemConfigure();
	updateObjectStatus();
}

SystemSetting::~SystemSetting()
{

}

void SystemSetting::on_btn_close_clicked(void)
{
	qint32 nTmp = 0;
	qint32 nFlag3, nFlag2, nFlag1, nFlag0;

	nFlag3 = ui.check_extra3->isChecked() ? 0x08 : 0;
	nFlag2 = ui.check_extra2->isChecked() ? 0x04 : 0;
	nFlag1 = ui.check_extra1->isChecked() ? 0x02 : 0;
	nFlag0 = ui.check_extra0->isChecked() ? 0x01 : 0;
	pickFilter = nFlag3 | nFlag2 | nFlag1 | nFlag0;
	
	bgAudio = ui.check_audio_bg->isChecked() ? 1 : 0;
	skillAudio = ui.check_audio_skill->isChecked() ? 1 : 0;
	WriteSystemConfigure();

	close();
}

void SystemSetting::ReadSystemConfigure(void)
{
	QSettings settings("./setting.ini", QSettings::IniFormat);

	pickFilter = settings.value("filter/extraAmount", 0x0F).toInt();

	bgAudio = settings.value("audio/backAudio").toInt();
	skillAudio = settings.value("audio/skillAudio").toInt();
}

void SystemSetting::WriteSystemConfigure(void)
{
	QSettings settings("./setting.ini", QSettings::IniFormat);

	settings.setValue("filter/extraAmount", pickFilter);
	settings.setValue("audio/backAudio", bgAudio);
	settings.setValue("audio/skillAudio", skillAudio);
}

void SystemSetting::updateObjectStatus(void)
{
	bool bTmp;

	bTmp = (0x08 & pickFilter) != 0;
	ui.check_extra3->setChecked(bTmp);

	bTmp = (0x04 & pickFilter) != 0;
	ui.check_extra2->setChecked(bTmp);

	bTmp = (0x02 & pickFilter) != 0;
	ui.check_extra1->setChecked(bTmp);

	bTmp = (0x01 & pickFilter) != 0;
	ui.check_extra0->setChecked(bTmp);

	ui.check_audio_bg->setChecked(bgAudio == 1);
	ui.check_audio_skill->setChecked(skillAudio == 1);
}