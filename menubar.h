#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>
#include <QMenu>

class MenuBar : public QMenuBar {
	Q_OBJECT

public:
	MenuBar(QWidget *parent = nullptr);	

private:
	friend class MainWindow;

	QMenu *userMenu;
	QAction *authorizeAction;

	QMenu *statisticsMenu;
};

#endif /* MENUBAR_H */
