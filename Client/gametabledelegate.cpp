/* -------------------------------------------------------------------------------------------- */
#include <QPainter>
#include "gametabledelegate.h"
#include "gametablemodel.h"
/* -------------------------------------------------------------------------------------------- */
GameTableDelegate::GameTableDelegate(QWidget* parent) :
	QStyledItemDelegate(parent)
{

}
/* -------------------------------------------------------------------------------------------- */
void GameTableDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const auto gameTableModel = qobject_cast<const GameTableModel*>(index.model());
	Q_ASSERT(gameTableModel);

	const auto value = gameTableModel->data(index).toInt();
	QPixmap pixmap;

	switch (value)
	{
	case GameTableModel::Values::Mine:
		pixmap.load(":/MainWindow/Resources/Mine.png");
		break;
	case GameTableModel::Values::ExplodedMine:
		pixmap.load(":/MainWindow/Resources/ExplodedMineCell.png");
		break;
	case GameTableModel::Values::Cell:
		if (option.state & QStyle::State_MouseOver)
			pixmap.load(":/MainWindow/Resources/CellOver.png");
		else
			pixmap.load(":/MainWindow/Resources/Cell.png");
		break;
	case GameTableModel::Values::EmptyCell:
		pixmap.load(":/MainWindow/Resources/EmptyCell.png");
		break;
	case GameTableModel::Values::One:
		pixmap.load(":/MainWindow/Resources/One.png");
		break;
	case GameTableModel::Values::Two:
		pixmap.load(":/MainWindow/Resources/Two.png");
		break;
	case GameTableModel::Values::Three:
		pixmap.load(":/MainWindow/Resources/Three.png");
		break;
	case GameTableModel::Values::Four:
		pixmap.load(":/MainWindow/Resources/Four.png");
		break;
	case GameTableModel::Values::Five:
		pixmap.load(":/MainWindow/Resources/Five.png");
		break;
	case GameTableModel::Values::Six:
		pixmap.load(":/MainWindow/Resources/Six.png");
		break;
	case GameTableModel::Values::Seven:
		pixmap.load(":/MainWindow/Resources/Seven.png");
		break;
	case GameTableModel::Values::Eight:
		pixmap.load(":/MainWindow/Resources/Eight.png");
		break;
	}

	const auto extra = 0;
	auto targetRect = option.rect.adjusted(0, 0, -extra, -extra);

	targetRect.setTop(targetRect.bottom() - 24 + 1);
	targetRect.setRight(targetRect.left() + 24 - 1);

	painter->drawPixmap(targetRect, pixmap);
}
/* -------------------------------------------------------------------------------------------- */
QSize GameTableDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QSize(24, 24);
}
/* -------------------------------------------------------------------------------------------- */