#ifndef ACTIVITIES_H
#define ACTIVITIES_H

#include "activity.h"

#include <QWidget>
#include <QTableWidget>

class Activities : public QTableWidget {
	Q_OBJECT
public:
	Activities(QWidget *parent = nullptr);
	void putActivity(Activity& activity);

};

#endif /* ACTIVITIES_H */
