/* -------------------------------------------------------------------------------------------- */
#pragma once
/* -------------------------------------------------------------------------------------------- */
#include <QDialog>
#include <QPointer>
/* -------------------------------------------------------------------------------------------- */
namespace Ui { class LoginDialog; }
class Client;
/* -------------------------------------------------------------------------------------------- */
class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LoginDialog(QWidget *parent = nullptr);
	~LoginDialog() override;

	QPointer<Client> login();

private slots:
	void updateActions();
	void findImage();
	void tryAccept();

private:
	Ui::LoginDialog* mUi;
	QRegExp mIpRegex;

	Client* mNewClient;
};
/* -------------------------------------------------------------------------------------------- */
