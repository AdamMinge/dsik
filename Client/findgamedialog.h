/* -------------------------------------------------------------------------------------------- */
#pragma once
/* -------------------------------------------------------------------------------------------- */
#include <QDialog>
/* -------------------------------------------------------------------------------------------- */
namespace Ui { class FindGameDialog; }
class Client;
class QMovie;
/* -------------------------------------------------------------------------------------------- */
class FindGameDialog : public QDialog
{
	Q_OBJECT

public:
	explicit FindGameDialog(QWidget *parent = nullptr);
	~FindGameDialog() override;

	bool findGame(Client* client);

private:
	Ui::FindGameDialog* mUi;

	Client* mClient;
};
/* -------------------------------------------------------------------------------------------- */
