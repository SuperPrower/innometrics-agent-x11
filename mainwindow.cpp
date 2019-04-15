#include "mainwindow.h"

#include <QMessageBox>
#include <QApplication>
#include <QDebug>

MainWindow::MainWindow() {
	if (this->objectName().isEmpty())
		this->setObjectName(QString::fromUtf8("MainWindow"));

	this->resize(400, 800);
	this->setAttribute(Qt::WA_DeleteOnClose, false);

	centralWidget = new QWidget();

	layout = new QVBoxLayout(centralWidget);
	layout->setContentsMargins(0, 0, 0, 0);

	QSizePolicy expandingPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	// sizePolicy.setHorizontalStretch(0);
	// sizePolicy.setVerticalStretch(0);
	// sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
	// centralWidget->setSizePolicy(sizePolicy);

	this->setCentralWidget(centralWidget);

	tabWidget = new QTabWidget(centralWidget);

	recentActivities = new Activities(this);
	recentActivities->setSizePolicy(expandingPolicy);

	tabWidget->addTab(recentActivities, "Recent Activities");

	allActivities = new Activities(this);
	allActivities->setSizePolicy(expandingPolicy);

	tabWidget->addTab(allActivities, "All Activities");

	layout->addWidget(tabWidget);

	menuBar = new MenuBar();
	menuBar->setGeometry(QRect(0, 0, 400, 25));
	this->setMenuBar(menuBar);

	mainToolBar = new QToolBar();
	mainToolBar->setMovable(false);
	this->addToolBar(Qt::TopToolBarArea, mainToolBar);

	statusBar = new QStatusBar();
	this->setStatusBar(statusBar);
	this->statusBar->showMessage("You are not logged in", 0);

	/* system tray icon and actions
	trayIconMenu = new QMenu();
	trayIconMenu->addAction("Show/Hide");
	trayIconMenu->addSeparator();
	trayIconMenu->addAction("Exit");
	*/

	trayIcon = new QSystemTrayIcon(this);
	// trayIcon->setIcon(QIcon::fromTheme("face-smile"));
	// trayIcon->setIcon(QIcon::fromTheme("document-new"));
	trayIcon->setIcon(QIcon("icon.png"));
	trayIcon->show();

	// Settings and DB location
	auto data_path = QStandardPaths::writableLocation(
		QStandardPaths::AppLocalDataLocation
	);
	// Unable to get Local Data Location
	if (data_path.length() == 0) {
		qDebug() << "Unable to get Local App Data Path" << endl;
		QApplication::exit(-1);
	}

	data_location = QDir(data_path);
	if (not data_location.exists())
		data_location.mkpath(data_location.absolutePath());

	qDebug() << "Data Location: " << data_location.absolutePath() << endl;

	// Sub-objects
	// TODO: set parents?
	// networking = new Networking("http://188.130.155.81:8120");
	networking = new Networking();
	ewmhClient = new EWMH_Client();
	ewmhClient->start();

	dbConnection = new DB_Connection(data_location.absoluteFilePath("activities.sqlite"));

	QSettings appSettings(data_location.absoluteFilePath("config"));
	/*
	if (appSettings.contains("token")) {
		networking->token = appSettings.value("token").toString();
		this->statusBar->showMessage("Authorized", 0);
	}
	*/

	// retrieve old activities
	for (auto a : dbConnection->getActivities()) {
		allActivities->putActivity(a);
	}

	activityStartTime = QDateTime::currentDateTime();

	// Temporarily Stored Activity
	wmClass = wName = "";

	/* prepare dialog boxes */
	loginDialog = new LoginDialog(this);
	registrationDialog = new RegistrationDialog(this);

	/* SIGNALS */

	// System Tray Icon
	connect(trayIcon, &QSystemTrayIcon::activated,
		this, [this]() {
			this->setVisible(!this->isVisible());
		}
	);

	// Menu buttons
	connect(menuBar->authorizeAction, &QAction::triggered,
		loginDialog, &LoginDialog::open
	);

	connect(menuBar->hideAction, &QAction::triggered,
		this, &QMainWindow::hide
	);

	connect(menuBar->exitAction, &QAction::triggered,
		this, &QApplication::exit
	);

	connect(loginDialog, &LoginDialog::wantRegister,
		registrationDialog, &RegistrationDialog::open
	);

	/* Networking */

	connect(loginDialog, &LoginDialog::acceptLogin,
		this, [this](QString e, QString p) {
		statusBar->showMessage("Authorizing User...", 0);
		// networking->login("sergey_test@test.test", "testing");
		networking->login(e, p);
	});

	connect(networking, &Networking::loginSuccessful, this, [this]() {
		loginDialog->close();
		QMessageBox result{this};
		result.setText("Authorization successful");
		result.exec();

		statusBar->showMessage("Authorized", 0);
		// save token
		/*
		QSettings appSettings(data_location.absoluteFilePath("config"));
		appSettings.setValue("token", networking->token);
		*/

	});

	connect(networking, &Networking::loginFailed,
		loginDialog, &LoginDialog::loginFailed
	);

	// Registration
	connect(registrationDialog, &RegistrationDialog::userRegister,
		this, [this](QString e, QString p, QString u, QString s) {
			registrationDialog->setEnabled(false);
			statusBar->showMessage("Registering User...", 0);
			networking->registerUser(e, p, u, s);
	});

	connect(networking, &Networking::registrationSuccessful, this, [this]() {
		registrationDialog->close();
		QMessageBox result{this};
		result.setText("Registration successful! Try to login now.");
		result.exec();
	});

	connect(networking, &Networking::registrationFailed,
		this, [this](QString msg) {
			registrationDialog->setEnabled(true);
			registrationDialog->showStatus(msg);
		}
	);

	// Activity sending

	connect(networking, &Networking::activitySendingError,
		this, [this](QString msg) {
			statusBar->showMessage("Unable to sync activity:" + msg);
		}
	);

	connect(networking, &Networking::activitySendingSuccess,
		this, [this](QString id) {
			/* TODO: need to somehow sync id's of sent
			   activities and their replied id's in DB
			   so that it's possible to sync missing
			   data to or from the server */
			statusBar->showMessage("Activity " + id + " sent");
		}
	);

	// EWMH Client
	connect(ewmhClient, &EWMH_Client::newActiveWindow, this, &MainWindow::newActivity);

}

// void MainWindow::setupSignals() { };

void MainWindow::newActivity(QString wmClass, QString wName) {
	// Record finished activity
	auto endTime = QDateTime::currentDateTime();

	auto timeElapsed = activityStartTime.secsTo(endTime);

	// only store non-empty activities with time elapsed more than 1 second
	if (not (this->wmClass.isEmpty() and this->wName.isEmpty()) and timeElapsed > 1) {

		QString format = "yyyy-MM-dd HH:mm:ss";

		Activity activity(
			this->wmClass.isEmpty() ? this->wName : this->wmClass,
			activityStartTime.toString(format),
			endTime.toString(format)
		);

		this->allActivities->putActivity(activity);
		this->recentActivities->putActivity(activity);
		this->dbConnection->putActivity(activity);

		if (networking->authorized) networking->sendActivity(activity);

	}

	// Store new activity
	this->wmClass = wmClass;
	this->wName = wName;
	this->activityStartTime = endTime;

}

void MainWindow::closeEvent(QCloseEvent *event) {
	// Hide to Tray
	event->ignore();
	this->hide();
}
