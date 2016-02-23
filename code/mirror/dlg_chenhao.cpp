#include "dlg_chenhao.h"
#include "Player.h"


extern QVector<Info_Chenhao> g_ChenhaoSet;

dlg_chenhao::dlg_chenhao(CHuman *w_Embodiment, QWidget *parent)
	: QDialog(parent), Embodiment(w_Embodiment)
{
	ui.setupUi(this);
	DisplayInfo();
	change = false;

	this->setWindowTitle(w_Embodiment->get_name() + QStringLiteral("的修炼进度"));
}

dlg_chenhao::~dlg_chenhao()
{

}

void dlg_chenhao::DisplayInfo()
{
	bool bHasNext = false;
	QString strTmp;
	const Info_Chenhao &ch = g_ChenhaoSet.value(Embodiment->get_xiulian());
	qint32 yuanli = Embodiment->get_yuanli();

	strTmp = QStringLiteral("需元力值：%1  当前拥有：%2").arg(ch.need).arg(yuanli);
	ui.lbl_status->setText(strTmp);

	//当前
	ui.lbl_cur_name->setText(ch.name);
	ui.lbl_cur_hp->setText(QString("%1").arg(0));
	ui.lbl_cur_dc->setText(QString("%1-%2").arg(ch.dc1).arg(ch.dc2));
	ui.lbl_cur_mc->setText(QString("%1-%2").arg(ch.mc1).arg(ch.mc2));
	ui.lbl_cur_sc->setText(QString("%1-%2").arg(ch.sc1).arg(ch.sc2));
	ui.lbl_cur_ac->setText(QString("%1").arg(0));
	ui.lbl_cur_mac->setText(QString("%1").arg(0));

	//下一阶
	Info_Chenhao nextCh;
	if (Embodiment->get_xiulian() < g_ChenhaoSet.count() - 1) {
		nextCh = g_ChenhaoSet.value(Embodiment->get_xiulian() + 1);
		bHasNext = true;
	} else {
		nextCh = { 0 };
		nextCh.name = QStringLiteral("未开放");
	}
	ui.lbl_next_name->setText(nextCh.name);
	ui.lbl_next_hp->setText(QString("%1").arg(0));
	ui.lbl_next_dc->setText(QString("%1-%2").arg(nextCh.dc1).arg(nextCh.dc2));
	ui.lbl_next_mc->setText(QString("%1-%2").arg(nextCh.mc1).arg(nextCh.mc2));
	ui.lbl_next_sc->setText(QString("%1-%2").arg(nextCh.sc1).arg(nextCh.sc2));
	ui.lbl_next_ac->setText(QString("%1").arg(0));
	ui.lbl_next_mac->setText(QString("%1").arg(0));

	ui.btn_xiulian->setEnabled(bHasNext && (yuanli >= ch.need));
}


void dlg_chenhao::on_btn_xiulian_clicked()
{
	qint32 need = g_ChenhaoSet.value(Embodiment->get_xiulian()).need;
	qint32 yuanli = Embodiment->get_yuanli();

	qint32 reserved1 = 0;
	if (Embodiment->get_xiulian() < g_ChenhaoSet.count()-1 && yuanli >= need)
	{
		Embodiment->sub_yuanli(need);
		Embodiment->add_xiulian();
		Embodiment->updateParameter();
		change = true;

		DisplayInfo();
	}
}
