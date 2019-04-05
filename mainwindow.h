#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "menubar.h"
#include "activities.h"
#include "networking.h"
#include "ewmh.h"
#include "db_connection.h"
#include "login_dialog.h"
#include "registration_dialog.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>

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

	QTabWidget *tabWidget;
	Activities *recentActivities;
	Activities *allActivities;

	MenuBar *menuBar;
	QToolBar *mainToolBar;
	QStatusBar *statusBar;

	// system tray
	QSystemTrayIcon *trayIcon;

	// Internal classes
	Networking *networking;
	EWMH_Client *ewmhClient;
	DB_Connection *dbConnection;

	// Settings and DB location
	QDir data_location;

	// Current Activity
	QString wmClass, wName;
	QDateTime activityStartTime;

	// Dialog Boxes
	LoginDialog *loginDialog;
	RegistrationDialog *registrationDialog;

};

#endif // MAINWINDOW_H
