#include "menubar.h"

MenuBar::MenuBar(QWidget *parent)
	: QMenuBar(parent)
{
	userMenu = new QMenu("User");
	statisticsMenu = new QMenu("Statistics");

	authorizeAction = userMenu->addAction("Login");

	this->addMenu(userMenu);
	this->addMenu(statisticsMenu);
}
