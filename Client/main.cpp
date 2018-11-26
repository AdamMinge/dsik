/* -------------------------------------------------------------------------------------------- */
#include <QApplication>
#include "logindialog.h"
#include "findgamedialog.h"
#include "client.h"
#include "gamedialog.h"
/* -------------------------------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	LoginDialog loginDialog;
	while(auto client = loginDialog.login())
	{
		FindGameDialog findGameDialog;
		const auto found = findGameDialog.findGame(client);

		if (found)
		{
			GameDialog gameDialog(client);
			gameDialog.exec();
		}

		client->disconnect();
	}

	return 0;
}
/* -------------------------------------------------------------------------------------------- */
