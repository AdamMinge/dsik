/* -------------------------------------------------------------------------------------------- */
#pragma once
/* -------------------------------------------------------------------------------------------- */
#include <QObject>
#include <array>
#include <QTcpSocket>
#include <QPixmap>
#include <QHostAddress>
/* -------------------------------------------------------------------------------------------- */
class Client : public QObject
{
	Q_OBJECT

public:
	struct ClientData
	{
		QString name;
		QPixmap image;
	};
	enum DataType
	{
		Empty = -1,
		SetName = 0,
		SetImage = 1,
		OpponentSetName = 2,
		OpponentSetImage = 3,
		FindGame = 4,
		YourRound = 6,
		MakeMove = 7,
		UpdateMap = 8,
		End = 9
	};

public:
	explicit Client(QObject *parent = nullptr);
	~Client() override;

	void setName(const QString& name);
	const QString& getName() const;

	void setImage(const QPixmap& image);
	const QPixmap& getImage() const;

	void findGame();
	bool myTurn() const;
	bool end() const;

	void makeMove(const QPoint& point);

	const QString& getOpponentName() const;
	const QPixmap& getOpponentImage() const;

	bool connect(QHostAddress host, quint16 port, int msecsWait);
	void disconnect();

signals:
	void foundGame();
	void gameEnded(bool end);
	void myTurnChanged(bool myTurn);
	void mapUpdated(const std::array<std::array<int, 50>, 50>& map);
	void nameChanged(const QString& name);
	void imageChanged(const QPixmap& image);
	void opponentNameChanged(const QString& name);
	void opponentImageChanged(const QPixmap& image);

private slots:
	void readyRead();
	void sendName(const QString& name);
	void sendImage(const QPixmap& image);

private:
	QTcpSocket mTcpSocket;

	int mDataType;
	int mRemainingBytes;
	QByteArray mReceiveData;

	ClientData mClientData;
	ClientData mOpponentData;

	bool mMyTurn;
	bool mEnd;
};
/* -------------------------------------------------------------------------------------------- */