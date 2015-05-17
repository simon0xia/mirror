#include "dlg_count.h"

dlg_count::dlg_count(QWidget *parent, const QString &title, quint32 nMax)
: QDialog(parent), m_nMax(nMax)
{
	ui.setupUi(this);

	this->setWindowTitle(title);
	m_nCount = m_nMax < 100 ? m_nMax : 100;
	ui.lineEdit->setText(QString::number(m_nCount));
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
	if (m_nCount > m_nMax)
	{
		m_nCount = m_nMax;
	}
	done(QDialog::Accepted);
}
void dlg_count::on_btn_cancel_clicked(void)
{
	done(QDialog::Rejected);
}
