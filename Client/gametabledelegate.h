/* -------------------------------------------------------------------------------------------- */
#pragma once
/* -------------------------------------------------------------------------------------------- */
#include <QStyledItemDelegate>
/* -------------------------------------------------------------------------------------------- */
class GameTableDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	explicit GameTableDelegate(QWidget *parent = nullptr);
	~GameTableDelegate() override = default;

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
/* -------------------------------------------------------------------------------------------- */