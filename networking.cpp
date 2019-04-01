#include "networking.h"

#include <QtNetwork>
#include <QJsonObject>
#include <QJsonValue>

Networking::Networking(QString baseUrl)
	: baseUrl(baseUrl)
{
	networkManager = new QNetworkAccessManager();
	// TODO: persistent cookie storage?
	networkManager->setCookieJar(new QNetworkCookieJar());

	// obtain hardware info
	for (auto i : QNetworkInterface::allInterfaces()) {
		auto flags = i.flags();

		// Skip loopback, invalid, down, not running and p2p devices
		if (flags & QNetworkInterface::IsLoopBack
		    || flags & QNetworkInterface::IsPointToPoint
		    || not (flags & QNetworkInterface::IsRunning)
		    || not (flags & QNetworkInterface::IsUp)
		    || not i.isValid()
		) continue;

		for (auto entry : i.addressEntries()) {
			if (i.hardwareAddress() != "00:00:00:00:00:00"
			    && entry.ip().toString().contains(".")
			) {
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

			// TODO: enable persistent cookie storage?
			this->authorized = true;

		} else if (err == QNetworkReply::TimeoutError) {
			emit this->loginFailed("Connection timed out");
		} else {
			emit this->loginFailed(data["message"].toString());
		}
	});
}

void Networking::registerUser(QString email, QString password, QString name, QString surname) {
	QNetworkRequest request { QUrl(baseUrl + "/login") };

	QHttpPart emailPart;
	emailPart.setHeader(
		QNetworkRequest::ContentDispositionHeader,
		QVariant("form-data; name=\"email\"")
	);
	emailPart.setBody(email.toUtf8());

	QHttpPart passwordPart;
	passwordPart.setHeader(
		QNetworkRequest::ContentDispositionHeader,
		QVariant("form-data; name=\"password\"")
	);
	passwordPart.setBody(password.toUtf8());

	QHttpPart namePart;
	namePart.setHeader(
		QNetworkRequest::ContentDispositionHeader,
		QVariant("form-data; name=\"name\"")
	);
	namePart.setBody(name.toUtf8());

	QHttpPart surnamePart;
	surnamePart.setHeader(
		QNetworkRequest::ContentDispositionHeader,
		QVariant("form-data; surname=\"surname\"")
	);
	surnamePart.setBody(surname.toUtf8());

	auto *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	multiPart->append(emailPart);
	multiPart->append(passwordPart);
	multiPart->append(namePart);
	multiPart->append(surnamePart);

	auto reply = networkManager->sendCustomRequest(
		request, QString("POST").toUtf8(), multiPart
	);

	connect(reply, &QNetworkReply::finished, this, [reply, this]() {
		auto err = reply->error();
		auto body = reply->readAll();

		auto data = QJsonDocument::fromJson(body);

		qDebug() << "POST /user response:"
			 << err << ", message:" << data["message"].toString() << endl;

		if (err == QNetworkReply::NoError) {
			emit this->registrationSuccessful();
			this->token = data["token"].toString();
		} else if (err == QNetworkReply::TimeoutError) {
			emit this->registrationFailed("Connection timed out");
		} else {
			emit this->registrationFailed(data["message"].toString());
		}
	});
}

void Networking::sendActivity(Activity activity) {
	QNetworkRequest request { QUrl(baseUrl + "/activity") };

	QJsonObject jsonActivity {
		{"activity", QJsonObject{
			{"start_time", activity.startTime},
			{"end_time", activity.endTime},
			{"activity_type", "os"},
			{"executable_name", activity.appName},
			{"ip_address", this->hostAddress},
			{"mac_address", this->hardwareAddress}
		}
	}};

	QByteArray data = QJsonDocument(jsonActivity).toJson();

	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.size()));

	auto reply = networkManager->post(request, data);

	connect(reply, &QNetworkReply::finished, this, [reply, this]() {
		auto err = reply->error();
		auto body = reply->readAll();

		auto data = QJsonDocument::fromJson(body);

		qDebug() << "POST /activity response:"
			 << err << ", message:" << data["message"].toString() << endl;

		if (err == QNetworkReply::NoError) {
			QString remoteId = data["_id"].toString();
			emit this->activitySendingSuccess(remoteId);

		} else if (err == QNetworkReply::TimeoutError) {
			emit this->activitySendingError("Connection timed out");
		} else {
			emit this->activitySendingError(data["message"].toString());
		}
	});
}
