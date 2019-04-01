#ifndef REGISTRATION_DIALOG_H
#define REGISTRATION_DIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class RegistrationDialog : public QDialog {
	Q_OBJECT;
public:
	RegistrationDialog(QWidget *parent = nullptr);

signals:
	void userRegister(QString email, QString password, QString name, QString surname);

public slots:
	void checkInputData();
	void showStatus(QString text);

private:

	QGridLayout *gridLayout = new QGridLayout();

	QLabel *labelName = new QLabel("Name");
	QLabel *labelSurname = new QLabel("Surname");
	QLabel *labelEmail = new QLabel("E-Mail");
	QLabel *labelPassword = new QLabel("Password");

	QLabel *message = new QLabel();;

	QLineEdit *editName = new QLineEdit();
	QLineEdit *editSurname = new QLineEdit();
	QLineEdit *editEmail = new QLineEdit();
	QLineEdit *editPassword = new QLineEdit();

	QPushButton *cancel = new QPushButton("Cancel");
	QPushButton *createAccount = new QPushButton("Register");
};


#endif // REGISTRATION_DIALOG_H
