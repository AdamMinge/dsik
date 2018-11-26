/* -------------------------------------------------------------------------------------------- */
#include "client.h"
#include <QBuffer>
/* -------------------------------------------------------------------------------------------- */
static QByteArray intToBytes(int value, int length)
{
	QByteArray byte;
	for (auto i = 0; i != length; ++i)
	{
		byte.append(static_cast<char>((value & (0xFF << (i * 8))) >> (i * 8)));
	}
	std::reverse(byte.begin(), byte.end());
	return byte;
}
/* -------------------------------------------------------------------------------------------- */
static int bytesToInt(const QByteArray& bytes)
{
	return bytes.toHex().toInt(nullptr, 16);
}
/* -------------------------------------------------------------------------------------------- */
Client::Client(QObject* parent) :
	QObject(parent),
	mTcpSocket(this),
	mDataType(Client::DataType::Empty),
	mRemainingBytes(0),
	mMyTurn(false),
	mEnd(false)
{

}
/* -------------------------------------------------------------------------------------------- */
Client::~Client()
{
	if (mTcpSocket.isOpen())
		mTcpSocket.close();
}
/* -------------------------------------------------------------------------------------------- */
void Client::setName(const QString& name)
{
	mClientData.name = name;
	emit nameChanged(mClientData.name);
}
/* -------------------------------------------------------------------------------------------- */
const QString& Client::getName() const
{
	return mClientData.name;
}
/* -------------------------------------------------------------------------------------------- */
void Client::setImage(const QPixmap& image)
{
	mClientData.image = image;
	emit imageChanged(mClientData.image);
}
/* -------------------------------------------------------------------------------------------- */
const QPixmap& Client::getImage() const
{
	return mClientData.image;
}
/* -------------------------------------------------------------------------------------------- */
void Client::findGame()
{
	QByteArray data;

	const auto length = intToBytes(0, 4);
	data.append(Client::DataType::FindGame);
	data.append(length);

	mTcpSocket.write(data);
}
/* -------------------------------------------------------------------------------------------- */
bool Client::myTurn() const
{
	return mMyTurn;
}
/* -------------------------------------------------------------------------------------------- */
bool Client::end() const
{
	return mEnd;
}
/* -------------------------------------------------------------------------------------------- */
void Client::makeMove(const QPoint& point)
{
	QByteArray data;

	const auto length = intToBytes(2, 4);
	data.append(Client::DataType::MakeMove);
	data.append(length);
	data.append(intToBytes(point.x(), 1));
	data.append(intToBytes(point.y(), 1));

	mTcpSocket.write(data);
}
/* -------------------------------------------------------------------------------------------- */
const QString& Client::getOpponentName() const
{
	return mOpponentData.name;
}
/* -------------------------------------------------------------------------------------------- */
const QPixmap& Client::getOpponentImage() const
{
	return mOpponentData.image;
}
/* -------------------------------------------------------------------------------------------- */
bool Client::connect(QHostAddress host, quint16 port, int msecsWait)
{
	mTcpSocket.connectToHost(host, port);
	const auto connected = mTcpSocket.waitForConnected(msecsWait);

	if (connected)
	{
		QObject::connect(&mTcpSocket, &QTcpSocket::readyRead, this, &Client::readyRead);
		QObject::connect(this, &Client::nameChanged, this, &Client::sendName);
		QObject::connect(this, &Client::imageChanged, this, &Client::sendImage);
	}

	return connected;
}
/* -------------------------------------------------------------------------------------------- */
void Client::disconnect()
{
	mTcpSocket.close();
}
/* -------------------------------------------------------------------------------------------- */
void Client::readyRead()
{
	const auto data = mTcpSocket.readAll();
	auto index = 0;

	while (index < data.length())
	{
		if (mDataType == Client::DataType::Empty)
		{
			mDataType = data[index];
			mRemainingBytes = bytesToInt(data.mid(index + 1, 4));
			index += 5;
		}
		else
		{
			const auto minLength = std::min(mRemainingBytes, data.length() - index);
			mRemainingBytes -= minLength;

			mReceiveData.append(data.mid(index, minLength));
			index += minLength;
		}

		if (mRemainingBytes == 0)
		{
			if (mDataType == Client::DataType::FindGame)
			{
				emit foundGame();
			}
			else if(mDataType == Client::DataType::OpponentSetName)
			{
				mOpponentData.name = mReceiveData;
				emit opponentNameChanged(mOpponentData.name);
			}
			else if(mDataType == Client::DataType::OpponentSetImage)
			{
				mOpponentData.image.loadFromData(mReceiveData, "PNG");
				emit opponentImageChanged(mOpponentData.image);
			}
			else if(mDataType == Client::DataType::YourRound)
			{
				mMyTurn = static_cast<bool>(bytesToInt(mReceiveData));
				emit myTurnChanged(mMyTurn);
			}
			else if(mDataType == Client::DataType::UpdateMap)
			{
				std::array<std::array<int, 50>, 50> map;

				for(auto x = 0; x < 50; ++x)
					for(auto y = 0; y < 50; ++y)
						map[x][y] = mReceiveData[x * 50 + y];

				emit mapUpdated(map);
			}
			else if(mDataType == Client::DataType::End)
			{
				mEnd = static_cast<bool>(bytesToInt(mReceiveData));
				emit gameEnded(mEnd);
			}

			mDataType = Client::DataType::Empty;
			mReceiveData.clear();
		}
	}
}
/* -------------------------------------------------------------------------------------------- */
void Client::sendName(const QString& name)
{
	QByteArray data;

	const auto length = intToBytes(mClientData.name.count(), 4);
	data.append(Client::DataType::SetName);
	data.append(length);
	data.append(mClientData.name);

	mTcpSocket.write(data);
}
/* -------------------------------------------------------------------------------------------- */
void Client::sendImage(const QPixmap& image)
{
	QByteArray data;
	QByteArray imageData;
	QBuffer buffer(&imageData);

	mClientData.image.save(&buffer, "PNG");

	const auto length = intToBytes(imageData.count(), 4);
	data.append(Client::DataType::SetImage);
	data.append(length);
	data.append(imageData);

	mTcpSocket.write(data);
}
/* -------------------------------------------------------------------------------------------- */
