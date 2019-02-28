#ifndef NETWORKING_H
#define NETWORKING_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>

class Networking : public QObject {
	Q_OBJECT
public:
	Networking(QString baseUrl = "https://innometric.guru:8120");

	void registerUser(QString email, QString password, QString name, QString surname);
	void deleteUser();

	void login(QString email, QString password);
	void logout();

signals:
	void loginSuccessful();
	void loginFailed(QString msg);

protected:
	bool lastSuccess;
	QString error;
	
	QString token;
	QString baseUrl;

	QNetworkAccessManager *networkManager;

	// hardware info
	QString hardwareAddress;
	QString hostAddress;
};

#endif // NETWORKING_H
