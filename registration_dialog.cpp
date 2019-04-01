#include "registration_dialog.h"


RegistrationDialog::RegistrationDialog(QWidget *parent)
	: QDialog(parent)
{
	editPassword->setEchoMode(QLineEdit::Password);

	gridLayout->addWidget(labelName,	0, 0);
	gridLayout->addWidget(editName,		0, 1, 1, -1);
	gridLayout->addWidget(labelSurname,	1, 0);
	gridLayout->addWidget(editSurname,	1, 1, 1, -1);
	gridLayout->addWidget(labelEmail,	2, 0);
	gridLayout->addWidget(editEmail,	2, 1, 1, -1);
	gridLayout->addWidget(labelPassword,	3, 0);
	gridLayout->addWidget(editPassword,	3, 1, 1, -1);

	message->hide();
	message->setStyleSheet("QLabel { color : red; }");

	gridLayout->addWidget(message,		4, 0, 1, -1);

	gridLayout->addWidget(cancel,		5, 0);
	gridLayout->addWidget(createAccount,	5, 1, 1, -1);

	this->setLayout(gridLayout);

	// Signals
	connect(cancel, &QPushButton::clicked, this, &RegistrationDialog::close);
	connect(
		createAccount, &QPushButton::clicked,
		this, &RegistrationDialog::checkInputData
	);
}

void RegistrationDialog::checkInputData() {
	if (editEmail->displayText().isEmpty()) {
		showStatus("Input e-mail");
	} else if (editPassword->displayText().isEmpty()) {
		showStatus("Input password");
	} else if (editName->displayText().isEmpty()) {
		showStatus("Input your name");
	} else if (editSurname->displayText().isEmpty()) {
		showStatus("Input your surname");
	} else {
		message->clear();
		message->hide();
		emit userRegister(
			editEmail->text(),
			editPassword->text(),
			editName->text(),
			editSurname->text()
		);
	}

}

void RegistrationDialog::showStatus(QString text) {
	message->setText(text);
	message->show();
}
