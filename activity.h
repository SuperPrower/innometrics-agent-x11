#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QString>
struct Activity {
	Activity() {}
	Activity(QString n, QString s, QString e) : appName(n), startTime(s), endTime(e) {}
	QString appName;
	QString startTime;
	QString endTime;
	// TODO: local id, remote id
};

#endif // ACTIVITY_H
