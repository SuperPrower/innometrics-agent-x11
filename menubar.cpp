#include "menubar.h"

MenuBar::MenuBar(QWidget *parent)
	: QMenuBar(parent)
{
	authorizeAction = userMenu->addAction("Login");
	userMenu->addSeparator();
	hideAction = userMenu->addAction("Hide");
	exitAction = userMenu->addAction("Close");

	this->addMenu(userMenu);
	this->addMenu(statisticsMenu);
}
