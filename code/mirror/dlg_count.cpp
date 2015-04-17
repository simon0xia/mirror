#include "dlg_count.h"

dlg_count::dlg_count(QWidget *parent, const QString &title, quint32 nMax)
: QDialog(parent), m_nMax(nMax)
{
	ui.setupUi(this);

	this->setWindowTitle(title);

//	ui.lineEdit->setText(QString::number(100));

//  在对话框运行时，lineEdit不可能失去焦点，故此响应无效。
//	connect(ui.lineEdit, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
}

dlg_count::~dlg_count()
{

}
// void dlg_count::editingFinished(void)
// {
// 	//编译完成时检测用户输入数量，不可超过最大数量。
// 	quint32 nTmp = ui.lineEdit->text().toUInt();
// 	if (nTmp > m_nMax)
// 	{		
// 		on_btn_max_clicked();
// 	}
// }
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
