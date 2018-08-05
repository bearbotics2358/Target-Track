/*

	MQTT subscriber

	created 8/2/18 BD - from example.cpp in mqtt examples

*/

#include <qmqtt.h>
#include <QCoreApplication>
#include <QTimer>
#include "subscriber.h"

Subscriber::Subscriber(const QHostAddress& host,
										const quint16 port,
										QObject* parent)
	: QMQTT::Client(host, port, parent), _qout(stdout)
{
	connect(this, &Subscriber::connected, this, &Subscriber::onConnected);
	connect(this, &Subscriber::subscribed, this, &Subscriber::onSubscribed);
	connect(this, &Subscriber::received, this, &Subscriber::onReceived);
}

void Subscriber::onConnected()
{
	_qout << "connected" << endl;
	subscribe(EXAMPLE_TOPIC, 0);
}

void Subscriber::onSubscribed(const QString& topic)
{
	_qout << "subscribed " << topic << endl;
}

void Subscriber::onReceived(const QMQTT::Message& message)
{
	_qout << "publish received: \"" << QString::fromUtf8(message.payload())
				<< "\"" << endl;
}

