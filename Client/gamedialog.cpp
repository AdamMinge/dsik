/* -------------------------------------------------------------------------------------------- */
#include "gamedialog.h"
#include "client.h"
#include "gametablemodel.h"
#include "gametabledelegate.h"
#include "ui_gamedialog.h"
/* -------------------------------------------------------------------------------------------- */
GameDialog::GameDialog(Client* client, QObject* parent) :
	mUi(new Ui::GameDialog),
	mClient(client),
	mModel(new GameTableModel(50, 50, this)),
	mDelegate(new GameTableDelegate(this))
{
	mUi->setupUi(this);

	connect(mClient, &Client::nameChanged, this, &GameDialog::nameChanged);
	connect(mClient, &Client::opponentNameChanged, this, &GameDialog::nameChanged);

	connect(mClient, &Client::imageChanged, this, &GameDialog::imageChanged);
	connect(mClient, &Client::opponentImageChanged, this, &GameDialog::imageChanged);

	connect(mClient, &Client::mapUpdated, this, &GameDialog::mapUpdated);
	connect(mClient, &Client::myTurnChanged, this, &GameDialog::myTurnChanged);
	connect(mClient, &Client::gameEnded, this, &GameDialog::gameEnded);

	connect(mUi->gameTableView, &QTableView::pressed, this, &GameDialog::pressed);

	mUi->yourAvatarLabel->setPixmap(mClient->getImage().scaled(64, 64));
	mUi->opponentAvatarLabel->setPixmap(mClient->getOpponentImage().scaled(64, 64));

	mUi->yourNameLineEdit->setText(mClient->getName());
	mUi->opponentNameLineEdit->setText(mClient->getOpponentName());

	mUi->gameTableView->setModel(mModel);
	mUi->gameTableView->setItemDelegate(mDelegate);
}
/* -------------------------------------------------------------------------------------------- */
void GameDialog::nameChanged()
{
	mUi->yourNameLineEdit->setText(mClient->getName());
	mUi->opponentNameLineEdit->setText(mClient->getOpponentName());
}
/* -------------------------------------------------------------------------------------------- */
void GameDialog::imageChanged()
{
	mUi->yourAvatarLabel->setPixmap(mClient->getImage().scaled(64, 64));
	mUi->opponentAvatarLabel->setPixmap(mClient->getOpponentImage().scaled(64, 64));
}
/* -------------------------------------------------------------------------------------------- */
void GameDialog::pressed(const QModelIndex& index)
{
	const auto value = mModel->data(index);
	if(value == GameTableModel::Values::Cell && mClient->myTurn())
	{
		const auto point = QPoint(index.row(), index.column());
		mClient->makeMove(point);
	}
}
/* -------------------------------------------------------------------------------------------- */
void GameDialog::myTurnChanged(bool myTurn)
{
	mUi->opponentBox->setEnabled(!myTurn);
	mUi->youBox->setEnabled(myTurn);
}
/* -------------------------------------------------------------------------------------------- */
void GameDialog::gameEnded(bool end)
{
	mUi->opponentBox->setEnabled(false);
	mUi->youBox->setEnabled(false);
}
/* -------------------------------------------------------------------------------------------- */
GameDialog::~GameDialog()
{
	delete mUi;
}
/* -------------------------------------------------------------------------------------------- */
void GameDialog::mapUpdated(const std::array<std::array<int, 50>, 50>& map)
{
	for(auto x = 0; x < 50; ++x)
	{
		for(auto y = 0; y < 50; ++y)
		{
			const auto index = mModel->index(x, y);
			mModel->setData(index, map[x][y]);
		}
	}
}
/* -------------------------------------------------------------------------------------------- */
