#ifndef DLG_CHENHAO_H
#define DLG_CHENHAO_H

#include <QDialog>
#include "ui_dlg_chenhao.h"
#include "Human.h"

class dlg_chenhao : public QDialog
{
	Q_OBJECT

public:
	dlg_chenhao(CHuman *w_Embodiment, QWidget *parent = 0);
	~dlg_chenhao();
	bool change;

private:
	void DisplayInfo();

private slots:
	void on_btn_xiulian_clicked(void);

private:
	Ui::dlg_chenhao ui;
	CHuman *Embodiment;
};

#endif // DLG_CHENHAO_H
