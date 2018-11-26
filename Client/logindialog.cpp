/* -------------------------------------------------------------------------------------------- */
#include <QFileInfo>
#include <QtGlobal>
#include <QMessageBox>
#include <QFileDialog>
#include "logindialog.h"
#include "ui_logindialog.h"
#include "client.h"
/* -------------------------------------------------------------------------------------------- */
LoginDialog::LoginDialog(QWidget* parent) :
	QDialog(parent),
	mUi(new Ui::LoginDialog),
	mNewClient(nullptr)
{
	mUi->setupUi(this);

	const QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
	mIpRegex = QRegExp("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");

	const auto ipValidator = new QRegExpValidator(mIpRegex, this);
	mUi->serverEdit->setValidator(ipValidator);

	connect(mUi->okButton, &QPushButton::pressed, this, &LoginDialog::tryAccept);
	connect(mUi->cancelButton, &QPushButton::pressed, this, &LoginDialog::reject);

	connect(mUi->browseButton, &QPushButton::pressed, this, &LoginDialog::findImage);
	connect(mUi->nameEdit, &QLineEdit::textChanged, this, &LoginDialog::updateActions);
	connect(mUi->avatarEdit, &QLineEdit::textChanged, this, &LoginDialog::updateActions);
	connect(mUi->serverEdit, &QLineEdit::textChanged, this, &LoginDialog::updateActions);

	updateActions();
}
/* -------------------------------------------------------------------------------------------- */
LoginDialog::~LoginDialog()
{
	if (mNewClient) mNewClient->deleteLater();
	delete mUi;
}
/* -------------------------------------------------------------------------------------------- */
QPointer<Client> LoginDialog::login()
{
	Q_ASSERT(!mNewClient);
	if (exec() == QDialog::Accepted)
	{
		const auto newClient = mNewClient;
		mNewClient = nullptr;
		return QPointer<Client>(newClient);
	}
	else return nullptr;
}
/* -------------------------------------------------------------------------------------------- */
void LoginDialog::updateActions()
{
	const auto name = mUi->nameEdit->text();
	const auto avatar = mUi->avatarEdit->text();
	const auto ip = mUi->serverEdit->text();

	mUi->okButton->setEnabled(!name.isEmpty() &&
		QFileInfo::exists(avatar) && mIpRegex.exactMatch(ip));
}
/* -------------------------------------------------------------------------------------------- */
void LoginDialog::findImage()
{
	auto avatar = QFileDialog::getOpenFileName(this,
		tr("Open Image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));

	if(!avatar.isEmpty())
		mUi->avatarEdit->setText(avatar);
}
/* -------------------------------------------------------------------------------------------- */
void LoginDialog::tryAccept()
{
	if (!mNewClient)
		mNewClient = new Client();

	QPixmap image;
	QHostAddress hostAddress;

	const auto name = mUi->nameEdit->text();
	const auto avatar = mUi->avatarEdit->text();
	const auto server = mUi->serverEdit->text();
	const auto port = mUi->portSpinBox->value();

	if(!image.load(avatar))
	{
		QMessageBox::critical(this, tr("Error"),
			tr("Loading image problem. "
				"Cannot Loading image (%1) !").arg(avatar));
		return;
	}

	hostAddress.setAddress(server);

	if (!mNewClient->connect(hostAddress, port, 1000))
	{
		QMessageBox::critical(this, tr("Error"),
			tr("Connect server problem. "
				"When using the given server address (%1) and "
				"port (%2) cannot connect with server!").arg(server).arg(port));
		return;
	}

	mNewClient->setName(name);
	mNewClient->setImage(image);

	accept();
}
/* -------------------------------------------------------------------------------------------- */
