#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include "activity.h"

#include <QtSql>
#include <QSqlDatabase>

#include <QVector>

class DB_Connection : public QObject {
	Q_OBJECT;
public:
	DB_Connection(QString dbPath, QObject *parent = nullptr);

	void putActivity(Activity& activity);
	QVector<Activity> getActivities();

protected:
	void initDb();

private:
	// database connection
	QSqlDatabase db;

};

#endif // DB_CONNECTION_H
