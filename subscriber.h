/*

	MQTT subscriber

	created 8/2/18 BD - from example.cpp in mqtt examples

*/

#include <qmqtt.h>
#include <QCoreApplication>
#include <QTimer>

const QHostAddress EXAMPLE_HOST = QHostAddress::LocalHost;
const quint16 EXAMPLE_PORT = 1183;
const QString EXAMPLE_TOPIC = "PI/CV/SHOOT/DATA";

class Subscriber : public QMQTT::Client
{
    Q_OBJECT
public:
		explicit Subscriber(const QHostAddress& host = EXAMPLE_HOST,
										const quint16 port = EXAMPLE_PORT,
										QObject* parent = NULL)		;
		
    virtual ~Subscriber() {} ;

    QTextStream _qout;

public slots:
	void onConnected();
	void onSubscribed(const QString& topic);
	void onReceived(const QMQTT::Message& message);
};

