#include "MiTableModel.h"
#include <QSize>


MiTableModel::MiTableModel(int row, int column, QObject *parent)
	: QAbstractTableModel(parent)
{
	row_Count = row;
	Col_Count = column;
	values.resize(row_Count * Col_Count);
}

MiTableModel::~MiTableModel()
{
	clear();
}

void MiTableModel::clear()
{
	beginResetModel();
	for (auto iter = values.begin(); iter != values.end(); iter++)
	{
		iter->id = 0;
	}
	endResetModel();
}

bool MiTableModel::isValid(const QModelIndex &index) const
{
	return (index.isValid()
		&& index.row() < row_Count
		&& index.column() < Col_Count);
}

const MiItem* MiTableModel::item(int row, int column) const
{
	return item(index(row, column));
}

const MiItem* MiTableModel::item(const QModelIndex &index) const
{
	if (!isValid(index))
		return nullptr;
	return &values.at(index.row() * Col_Count + index.column());
}

inline int MiTableModel::rowCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : row_Count;
}
inline int MiTableModel::columnCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : Col_Count;
}

QVariant MiTableModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::SizeHintRole)
	{
		return QSize(40, 40);
	}
	if (role == Qt::DisplayRole)
	{
		const MiItem *p = item(index);
		if (p != nullptr)
		{
			return QVariant((qint64)p);
		}
	}
	return QVariant();
}

void MiTableModel::setData(int row, int col, MiItem &item)
{
	QModelIndex Mi = index(row, col);
	if (isValid(Mi))
	{
		values[row * Col_Count + col] = item;
		dataChanged(Mi, Mi);
	}
}
