#ifndef DLG_COUNT_H
#define DLG_COUNT_H

#include <QDialog>
#include "ui_dlg_count.h"

class dlg_count : public QDialog
{
	Q_OBJECT

public:
	dlg_count(QWidget *parent, const QString &title, quint32 nMax);
	~dlg_count();
	quint32 getCount(void) { return m_nCount; };


private slots:
	void on_btn_max_clicked(void);
	void on_btn_ok_clicked(void);
	void on_btn_cancel_clicked(void);


private:
	Ui::dlg_count ui;

	quint32 m_nMax, m_nCount;
};

#endif // DLG_COUNT_H
