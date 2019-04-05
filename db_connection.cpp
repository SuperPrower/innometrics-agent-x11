#include "db_connection.h"

#include <QDebug>
#include <QApplication>

DB_Connection::DB_Connection(QString dbPath, QObject *parent)
	: QObject(parent)
{
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(dbPath);

	if (!db.open()) {
		qDebug() << "Error opening a database: " << db.lastError().text() << endl;
		QApplication::exit(-1);
	}

	this->initDb();

}

void DB_Connection::initDb() {
	QSqlQuery query(db);
	QString qstr = "CREATE TABLE IF NOT EXISTS activity ("
		"activity_id INTEGER PRIMARY KEY NOT NULL,"
		"remote_id VARCHAR(127),"
		"activity_type VARCHAR(127) NOT NULL,"
		"executable_name VARCHAR(255) NOT NULL,"
		"start_time VARCHAR(127) NOT NULL,"
		"end_time VARCHAR(127) NOT NULL"
		");";

	if (not query.exec(qstr)) {
		qDebug() << "Error creating a table: " << query.lastError().text() << endl;
		QApplication::exit(-1);
	}
}

void DB_Connection::putActivity(Activity& activity) {
	QSqlQuery query(db);
	QString qstr = "INSERT INTO activity"
		"(activity_type, executable_name, start_time, end_time)"
		"VALUES ('%1', '%2', '%3', '%4');";
	QString qf = qstr
		.arg("os")
		.arg(activity.appName)
		.arg(activity.startTime)
		.arg(activity.endTime);
	if (not query.exec(qf)) {
		qDebug() << "Error in Insert Query: " << query.lastError().text() << endl;
		QApplication::exit(-1);
	}
}

// TODO
// void DB_Connection::putActivityRemoteId(int id, QString remoteId);

QVector<Activity> DB_Connection::getActivities() {
	QVector<Activity> res;
	QSqlQuery query(db);
	QString qstr = "SELECT executable_name, start_time, end_time FROM activity;";

	if (not query.exec(qstr)) {
		qDebug() << "Error in Select Query: " << query.lastError().text() << endl;
		QApplication::exit(-1);
	}
	while (query.next()) {
		res.append(Activity(
			query.value(0).toString(),
			query.value(1).toString(),
			query.value(2).toString()
		));
	}

	return res;
}
