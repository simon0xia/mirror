#ifndef BAGMODEL_H
#define BAGMODEL_H

#include <QAbstractListModel>
#include <QImage>
#include "def_DataType.h"

class MiItem
{
public:
	MiItem(void) { id=0; }

public:
	itemID id;
	qint32 count;
	qint32 intensify;
	QImage image;
	QImage quality;
};

class MiTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	MiTableModel(int row, int column, QObject *parent = 0);
	~MiTableModel();

	void clear();

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	void setData(int row, int col, MiItem &item);

	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;

private:
	bool isValid(const QModelIndex &index) const;
	const MiItem* item(int row, int column) const;
	const MiItem* item(const QModelIndex &index) const;

private:
	QVector<MiItem> values;
	int row_Count, Col_Count;
};

#endif // BAGMODEL_H
