/* -------------------------------------------------------------------------------------------- */
#include <QIcon>
#include "gametablemodel.h"
/* -------------------------------------------------------------------------------------------- */
GameTableModel::GameTableModel(int width, int height, QObject* parent) :
	QAbstractTableModel(parent),
	mSize(width, height)
{
	mTable.resize(mSize.width());
	for(auto x = 0; x < mSize.width(); ++x)
	{
		mTable[x].resize(mSize.height());
		for(auto y = 0; y < mSize.height(); ++y)
		{
			mTable[x][y] = Values::Cell;
		}
	}
}
/* -------------------------------------------------------------------------------------------- */
int GameTableModel::rowCount(const QModelIndex& parent) const
{
	return mSize.height();
}
/* -------------------------------------------------------------------------------------------- */
int GameTableModel::columnCount(const QModelIndex& parent) const
{
	return mSize.width();
}
/* -------------------------------------------------------------------------------------------- */
QVariant GameTableModel::data(const QModelIndex& index, int role) const
{
	const auto row = index.row();
	const auto col = index.column();
	 
	if(role == Qt::DisplayRole)
		return mTable[row][col];

	return QVariant();
}
/* -------------------------------------------------------------------------------------------- */
bool GameTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	const auto row = index.row();
	const auto col = index.column();

	if (role == Qt::EditRole)
	{
		mTable[row][col] = value.toInt();
		emit dataChanged(index, index);
	}
		
	return true;
}
/* -------------------------------------------------------------------------------------------- */
Qt::ItemFlags GameTableModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsEnabled;
}
/* -------------------------------------------------------------------------------------------- */
