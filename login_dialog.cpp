#include "login_dialog.h"

LoginDialog::LoginDialog(QWidget *parent)
	:QDialog(parent)
{
	editPassword->setEchoMode(QLineEdit::Password);

	gridLayout->addWidget(labelEmail,	0, 0);
	gridLayout->addWidget(editEmail,	0, 1, 1, -1);
	gridLayout->addWidget(labelPassword,	1, 0);
	gridLayout->addWidget(editPassword,	1, 1, 1, -1);

	message->hide();
	message->setStyleSheet("QLabel { color : red; }");

	gridLayout->addWidget(message,		2, 0, 1, -1);

	newAccount = new QPushButton("Register");
	cancel = new QPushButton("Cancel");
	login = new QPushButton("Login");

	gridLayout->addWidget(newAccount,	3, 0);
	gridLayout->addWidget(cancel,		3, 1);
	gridLayout->addWidget(login,		3, 2);

	this->setLayout(gridLayout);

	// Signals
	connect(newAccount, &QPushButton::clicked, this, &LoginDialog::wantRegister);
	connect(cancel, &QPushButton::clicked, this, &LoginDialog::close);
	connect(login, &QPushButton::clicked, this, &LoginDialog::checkLogin);
}

void LoginDialog::checkLogin() {
	if (editEmail->displayText().isEmpty()) {
		message->setText("Input e-mail");
		message->show();
	} else if (editPassword->displayText().isEmpty()) {
		message->setText("Input password");
		message->show();
	} else {
		message->clear();
		message->hide();
		this->setEnabled(false);
		emit acceptLogin(editEmail->text(), editPassword->text());
	}
}

void LoginDialog::loginFailed(QString reason) {
	this->setEnabled(true);
	message->setText(reason);
	message->show();
}
