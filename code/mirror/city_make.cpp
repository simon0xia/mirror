#include "city_make.h"

city_make::city_make(QWidget *parent, CPlayer *const w_player)
	: QWidget(parent), player(w_player)
{
	ui.setupUi(this);

	m_bag_equip = player->get_bag_equip();
	m_bag_item = player->get_bag_item();
}

city_make::~city_make()
{

}

void city_make::mousePressEvent(QMouseEvent *ev)
{
	mouseMovePos = ev->globalPos();
}
void city_make::mouseMoveEvent(QMouseEvent * ev)
{
	if (mouseMovePos != QPoint(0, 0))
	{
		move(this->pos() + ev->globalPos() - mouseMovePos);
		mouseMovePos = ev->globalPos();
	}
}
void city_make::mouseReleaseEvent(QMouseEvent *ev)
{
	mouseMovePos = QPoint(0, 0);
}

void city_make::on_btn_close_clicked(void)
{
	close();
}

void city_make::updateInfo(void)
{

}
