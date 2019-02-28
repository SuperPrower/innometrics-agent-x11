#include "activities.h"

#include <QHeaderView>

Activities::Activities(QWidget *parent)
	: QTableWidget(parent)
{
	this->setColumnCount(4);
	this->setSortingEnabled(false);

	QStringList header;
	header << "WM Class" << "Window Name" << "Start Time" << "End Time";
	this->setHorizontalHeaderLabels(header);
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	// hide id's
	this->verticalHeader()->hide();

	// disable editing
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void Activities::putActivity(QString wmClass, QString wName, QString startTime, QString endTime) {
	this->insertRow(0);
	this->setItem(0, 0, new QTableWidgetItem(wmClass));
	this->setItem(0, 1, new QTableWidgetItem(wName));
	this->setItem(0, 2, new QTableWidgetItem(startTime));
	this->setItem(0, 3, new QTableWidgetItem(endTime));
}
