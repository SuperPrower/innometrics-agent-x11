#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QGridLayout>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class LoginDialog : public QDialog {
	Q_OBJECT
public:
	LoginDialog(QWidget *parent = nullptr);

signals:
	void wantRegister();
	void acceptLogin(QString username, QString password);

public slots:
	void loginFailed(QString reason);
	void checkLogin();

private:
	
	QGridLayout *gridLayout;

	QLabel *labelUsername;
	QLabel *labelPassword;

	QLabel *message;

	QLineEdit *editUsername;
	QLineEdit *editPassword;

	QPushButton *newAccount;
	QPushButton *cancel;
	QPushButton *login;

	
};

#endif /* LOGIN_DIALOG_H */
