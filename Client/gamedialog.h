/* -------------------------------------------------------------------------------------------- */
#pragma once
/* -------------------------------------------------------------------------------------------- */
#include <QDialog>
#include <QPixmap>
#include <QGraphicsView>
/* -------------------------------------------------------------------------------------------- */
namespace Ui { class GameDialog; }
class Client;
class GameTableModel;
class GameTableDelegate;
/* -------------------------------------------------------------------------------------------- */
class GameDialog : public QDialog
{
	Q_OBJECT

public:
	explicit GameDialog(Client* client, QObject* parent = nullptr);
	~GameDialog() override;

private slots:
	void mapUpdated(const std::array<std::array<int, 50>, 50>& map);
	void nameChanged();
	void imageChanged();
	void pressed(const QModelIndex& index);
	void myTurnChanged(bool myTurn);
	void gameEnded(bool end);

private:
	Ui::GameDialog* mUi;

	Client* mClient;
	GameTableModel* mModel;
	GameTableDelegate* mDelegate;
};
/* -------------------------------------------------------------------------------------------- */
