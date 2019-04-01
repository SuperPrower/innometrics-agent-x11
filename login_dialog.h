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
	void acceptLogin(QString email, QString password);

public slots:
	void loginFailed(QString reason);
	void checkLogin();

private:

	QGridLayout *gridLayout = new QGridLayout();;

	QLabel *labelEmail = new QLabel("E-Mail");
	QLabel *labelPassword = new QLabel("Password");

	QLabel *message = new QLabel();

	QLineEdit *editEmail = new QLineEdit();
	QLineEdit *editPassword = new QLineEdit();

	QPushButton *newAccount;
	QPushButton *cancel;
	QPushButton *login;


};

#endif /* LOGIN_DIALOG_H */
