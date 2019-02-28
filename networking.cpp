#include "networking.h"

#include <QtNetwork>
#include <QJsonObject>

Networking::Networking(QString baseUrl)
	: baseUrl(baseUrl)
{
	networkManager = new QNetworkAccessManager();

	// obtain hardware info
	for (auto i : QNetworkInterface::allInterfaces()) {
		auto flags = i.flags();

		// Skip loopback, invalid, down, not running and p2p devices
		if (flags & QNetworkInterface::IsLoopBack
		    || flags & QNetworkInterface::IsPointToPoint
		    || not flags & QNetworkInterface::IsRunning
		    || not flags & QNetworkInterface::IsUp
		    || not i.isValid()
		) continue;

		for (auto entry : i.addressEntries()) {
			if (i.hardwareAddress() != "00:00:00:00:00:00"
			    && entry.ip().toString().contains(".")
			) {
				// qInfo() << i.name() << " "
				// 	<< entry.ip().toString() << " "
				// 	<< i.hardwareAddress();

				// take first suitable interface and stop
				hardwareAddress = i.hardwareAddress();
				hostAddress = entry.ip().toString();
				goto exit;
			}
		}
	}
exit:
	;

}


void Networking::login(QString email, QString password) {
	QNetworkRequest request { QUrl(baseUrl + "/login") };

	QHttpPart emailPart;
	emailPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"email\""));
	emailPart.setBody(email.toUtf8());

	QHttpPart passwordPart;
	passwordPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"password\""));
	passwordPart.setBody(password.toUtf8());

	auto *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	multiPart->append(emailPart);
	multiPart->append(passwordPart);

	auto reply = networkManager->sendCustomRequest(request, QString("GET").toUtf8(), multiPart);

	connect(reply, &QNetworkReply::finished, this, [reply, this]() {
		auto err = reply->error();
		auto body = reply->readAll();

		auto data = QJsonDocument::fromJson(body);

		qDebug() << "/login response:"
			 << err << ", message:" << data["message"].toString() << endl;

		if (err == QNetworkReply::NoError) {
			emit this->loginSuccessful();
			this->token = data["token"].toString();
		} else {
			emit this->loginFailed(data["message"].toString());
		}
	});



}
