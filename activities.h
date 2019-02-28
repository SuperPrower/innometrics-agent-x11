#ifndef ACTIVITIES_H
#define ACTIVITIES_H

#include <QWidget>
#include <QTableWidget>

class Activities : public QTableWidget {
	Q_OBJECT
public:
	Activities(QWidget *parent = nullptr);
	void putActivity(QString wmClass, QString wName, QString startTime, QString endTime);

};

#endif /* ACTIVITIES_H */
