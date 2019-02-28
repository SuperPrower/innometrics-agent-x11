#include "mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow() {
	if (this->objectName().isEmpty())
		this->setObjectName(QString::fromUtf8("MainWindow"));

	this->resize(400, 800);

	centralWidget = new QWidget();
	centralWidget->setObjectName(QString::fromUtf8("centralWidget"));

	layout = new QVBoxLayout(centralWidget);
	layout->setContentsMargins(0, 0, 0, 0);

	QSizePolicy expandingPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	// sizePolicy.setHorizontalStretch(0);
	// sizePolicy.setVerticalStretch(0);
	// sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
	// centralWidget->setSizePolicy(sizePolicy);

	this->setCentralWidget(centralWidget);

	// tabWidget = new QTabWidget(centralWidget);
	// tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
	// layout->addWidget(tabWidget);

	// tab = new QWidget();
	// tab->setObjectName(QString::fromUtf8("tab"));
	// tabWidget->addTab(tab, QString());

	// tab_2 = new QWidget();
	// tab_2->setObjectName(QString::fromUtf8("tab_2"));
	// tabWidget->addTab(tab_2, QString());

	activities = new Activities(this);
	activities->setSizePolicy(expandingPolicy);

	layout->addWidget(activities);

	menuBar = new MenuBar();
	menuBar->setObjectName(QString::fromUtf8("menuBar"));
	menuBar->setGeometry(QRect(0, 0, 400, 25));
	this->setMenuBar(menuBar);

	mainToolBar = new QToolBar();
	mainToolBar->setObjectName(QString::fromUtf8("toolBar"));
	mainToolBar->setMovable(false);
	this->addToolBar(Qt::TopToolBarArea, mainToolBar);

	statusBar = new QStatusBar();
	statusBar->setObjectName(QString::fromUtf8("statusBar"));
	this->setStatusBar(statusBar);

	/* system tray icon and actions */
	trayIconMenu = new QMenu();
	trayIconMenu->addSeparator();

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
	trayIcon->setIcon(QIcon::fromTheme("face-smile"));
	trayIcon->show();

	// Sub-objects
	networking = new Networking();
	ewmhClient = new EWMH_Client();
	ewmhClient->start();

	activityStartTime = QDateTime::currentDateTime();

	// Temporarily Stored Activity
	wmClass = wName = "";

	/* prepare dialog boxes */
	loginDialog = new LoginDialog(this);
	
	/* SIGNALS */

	// login button 
	connect(menuBar->authorizeAction, &QAction::triggered,
		loginDialog, &LoginDialog::open
	);

	connect(loginDialog, &LoginDialog::acceptLogin,
		this, [this](QString u, QString p) {
		statusBar->showMessage("Authorizing User...", 0);
		// networking->login("sergey_test@test.test", "testing");
		networking->login(u, p);
	}); 

	// signal for login action
	connect(networking, &Networking::loginSuccessful, this, [this]() {
		loginDialog->close();
		QMessageBox result{this};
		result.setText("Authorization successful");
		result.exec();
	});

	connect(networking, &Networking::loginFailed, loginDialog, &LoginDialog::loginFailed);

	// window change action
	connect(ewmhClient, &EWMH_Client::newActiveWindow, this, &MainWindow::newActivity);
	
}

// void MainWindow::setupSignals() { };

void MainWindow::newActivity(QString wmClass, QString wName) {
	// Record finished activity
	auto endTime = QDateTime::currentDateTime();

	auto timeElapsed = activityStartTime.secsTo(endTime);

	// only store non-empty activities with time elapsed more than 5 seconds
	if (not (this->wmClass.isEmpty() and this->wName.isEmpty()) and timeElapsed > 5) {

		QString format = "";

		if (timeElapsed < 86'400) {
			format = "hh:mm:ss";

		} else {
			format = "";
		}

		activities->putActivity(
			this->wmClass, this->wName,
			activityStartTime.toString(format),
			endTime.toString(format)
		);

		// TODO: add this record to DB
	}

	// Store new activity
	this->wmClass = wmClass;
	this->wName = wName;
	this->activityStartTime = endTime;

}

void MainWindow::closeEvent(QCloseEvent *event) {
	// Hide to Tray	
}
