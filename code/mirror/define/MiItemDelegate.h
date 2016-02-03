#ifndef MI_ITEM_DELEGATE_H
#define MI_ITEM_DELEGATE_H

#include <QItemDelegate>

//绘制装备。单元格背景为装备品质框。然后显示中央显示装备，右下角显示数量，右上角显示强化次数
class MiItemDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	MiItemDelegate(QObject *parent = 0) : QItemDelegate(parent) { }
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index) const;
	void drawDisplay(QPainter *painter, const QPoint &pos, const QSize &ItemGridSize, QVariant value) const;
};

#endif // MI_ITEM_DELEGATE_H
