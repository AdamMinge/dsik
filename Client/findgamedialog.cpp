/* -------------------------------------------------------------------------------------------- */
#include <QMovie>
#include "findgamedialog.h"
#include "ui_findgamedialog.h"
#include "client.h"
/* -------------------------------------------------------------------------------------------- */
FindGameDialog::FindGameDialog(QWidget* parent) :
	QDialog(parent),
	mUi(new Ui::FindGameDialog),
	mClient(nullptr)
{
	mUi->setupUi(this);

	auto movie = new QMovie(":/MainWindow/Resources/loader.gif", QByteArray(), this);
	Q_ASSERT(movie->isValid());

	mUi->findingPlayerGifLabel->setMovie(movie);
	movie->start();
}
/* -------------------------------------------------------------------------------------------- */
FindGameDialog::~FindGameDialog()
{
	delete mUi;
}
/* -------------------------------------------------------------------------------------------- */
bool FindGameDialog::findGame(Client* client)
{
	mClient = client;
	connect(mClient, &Client::foundGame, this, &FindGameDialog::accept);

	mClient->findGame();
	if (exec() == QDialog::Accepted)
	{
		return true;
	}
	else return false;
}
/* -------------------------------------------------------------------------------------------- */
