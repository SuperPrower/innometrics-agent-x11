#include "activities.h"

#include <QHeaderView>

Activities::Activities(QWidget *parent)
	: QTableWidget(parent)
{
	this->setColumnCount(3);
	this->setSortingEnabled(false);

	QStringList header;
	header << "Application Name" << "Start Time" << "End Time";
	this->setHorizontalHeaderLabels(header);
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	// hide id's
	this->verticalHeader()->hide();

	// disable editing
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void Activities::putActivity(Activity& activity) {
	this->insertRow(0);
	this->setItem(0, 0, new QTableWidgetItem(activity.appName));
	this->setItem(0, 1, new QTableWidgetItem(activity.startTime));
	this->setItem(0, 2, new QTableWidgetItem(activity.endTime));
}
