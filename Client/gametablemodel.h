/* -------------------------------------------------------------------------------------------- */
#pragma once
/* -------------------------------------------------------------------------------------------- */
#include <QAbstractTableModel>
#include <QStyleOptionViewItem>
#include <QSize>
/* -------------------------------------------------------------------------------------------- */
class GameTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum Values
	{
		EmptyCell = 0,
		One = 1,
		Two = 2,
		Three = 3,
		Four = 4,
		Five = 5,
		Six = 6,
		Seven = 7,
		Eight = 8,
		Mine = 9,
		ExplodedMine = 10,
		Cell = 11
	};

public:
	explicit GameTableModel(int width, int height, QObject *parent = nullptr);
	~GameTableModel() override = default;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
	Qt::ItemFlags flags(const QModelIndex & index) const override;

private:
	QVector<QVector<int>> mTable;
	QSize mSize;
};
/* -------------------------------------------------------------------------------------------- */
