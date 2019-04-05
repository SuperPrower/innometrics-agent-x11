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

	QMenu *userMenu = new QMenu("User");
	QAction *authorizeAction;
	QAction *hideAction;
	QAction *exitAction;

	QMenu *statisticsMenu = new QMenu("Statistics");
};

#endif /* MENUBAR_H */
