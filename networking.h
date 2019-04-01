#ifndef NETWORKING_H
#define NETWORKING_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>

#include "activity.h"

class Networking : public QObject {
	Q_OBJECT
public:
	Networking(QString baseUrl = "https://innometric.guru:8120");

	void login(QString email, QString password);
	void registerUser(QString email, QString password, QString name, QString surname);
	// void deleteUser();
	// void logout();

	void sendActivity(Activity activity);


signals:
	void loginSuccessful();
	void loginFailed(QString msg);

	void registrationSuccessful();
	void registrationFailed(QString msg);

	void activitySendingSuccess(QString remoteId);
	void activitySendingError(QString msg);

protected:
	friend class MainWindow;

	bool lastSuccess;
	QString error;

	QString token;
	bool authorized = false;

	QString baseUrl;

	QNetworkAccessManager *networkManager;

	// hardware info
	QString hardwareAddress;
	QString hostAddress;
};

#endif // NETWORKING_H
