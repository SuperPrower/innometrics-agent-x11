#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "menubar.h"
#include "activities.h"
#include "networking.h"
#include "ewmh.h"
#include "login_dialog.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

public slots:
	void newActivity(QString wmClass, QString wName);

protected:
	void closeEvent(QCloseEvent *event) override;

private:
	QWidget *centralWidget;
	QVBoxLayout *layout;

	// main window
	// QTabWidget *tabWidget;
	// QWidget *tab;		// Recent Activities
	// QWidget *tab_2;		// This Session Activities
	Activities *activities;

	MenuBar *menuBar;
	QToolBar *mainToolBar;
	QStatusBar *statusBar;

	// system tray
	QSystemTrayIcon *trayIcon;
	QMenu *trayIconMenu;

	// 
	Networking *networking;
	EWMH_Client *ewmhClient;

	// Current Activity
	QString wmClass, wName;
	QDateTime activityStartTime;

	// Dialog Boxes
	LoginDialog *loginDialog;

};

#endif // MAINWINDOW_H
