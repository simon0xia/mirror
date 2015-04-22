#include "dlg_count.h"

dlg_count::dlg_count(QWidget *parent, const QString &title, quint32 nMax)
: QDialog(parent), m_nMax(nMax)
{
	ui.setupUi(this);

	this->setWindowTitle(title);

//	ui.lineEdit->setText(QString::number(100));
}

dlg_count::~dlg_count()
{

}

void dlg_count::on_btn_max_clicked(void)
{
	ui.lineEdit->setText(QString::number(m_nMax));
}
void dlg_count::on_btn_ok_clicked(void)
{
	m_nCount = ui.lineEdit->text().toUInt();
	done(QDialog::Accepted);
}
void dlg_count::on_btn_cancel_clicked(void)
{
	done(QDialog::Rejected);
}
