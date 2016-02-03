#include "MiItemDelegate.h"
#include "MiTableModel.h"
#include <QPainter>

void MiItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
	const QModelIndex & index) const
{
	Q_ASSERT(index.isValid());

	QStyleOptionViewItem opt = setOptions(index, option);

	QVariant value;

	drawBackground(painter, opt, index);

	QSize ItemGridSize = index.data(Qt::SizeHintRole).toSize();

	value = index.data(Qt::DisplayRole);
	if (value.isValid() && !value.isNull())
	{
		drawDisplay(painter, option.rect.center(), ItemGridSize, value);
	}

	drawFocus(painter, opt, option.rect);
}

void MiItemDelegate::drawDisplay(QPainter *painter, const QPoint &posCenter, const QSize &ItemGridSize, QVariant value) const
{
	MiItem *nPtr = (MiItem *)value.toLongLong();
	if (nPtr->id <= 0)
	{
		return;
	}

	QPoint ptbg = posCenter - QPoint(ItemGridSize.width() / 2, ItemGridSize.height() / 2);
	painter->drawImage(ptbg, nPtr->quality);

	QPoint ptItem = posCenter - QPoint(nPtr->image.width() / 2, nPtr->image.height() / 2);
	painter->drawImage(ptItem, nPtr->image);

	QRect rtEq = QRect(ptbg, ItemGridSize);
	if (nPtr->count > 1)
	{
		QString StrTmp = QStringLiteral("x%1 ").arg(nPtr->count);
		painter->drawText(rtEq, Qt::AlignRight | Qt::AlignBottom, StrTmp);
	}

	if (nPtr->intensify > 0)
	{
		QString StrTmp = QStringLiteral("+%1 ").arg(nPtr->intensify);
		painter->drawText(rtEq, Qt::AlignRight | Qt::AlignBottom, StrTmp);
	}
}