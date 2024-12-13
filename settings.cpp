#include "Settings.h"
#include "ui_settings.h"
#include <QMessageBox>

Settings::Settings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);

    ui->positionComboBox->clear();
    ui->positionComboBox->addItem("User");
    ui->positionComboBox->addItem("Developer");
    ui->positionComboBox->addItem("Admin");

    connect(ui->savePushButton, &QPushButton::clicked, this, &Settings::onSaveClicked);
    connect(ui->cancelPushButton, &QPushButton::clicked, this, &Settings::onCancelClicked);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::setUserData(const QString &username, const QString &password,
                           const QString &firstname, const QString &lastname, const QString &position)
{
    ui->usernameLineEdit->setText(username);
    ui->passwordLineEdit->setText(password);
    ui->firstNameLineEdit->setText(firstname);
    ui->lastNameLineEdit->setText(lastname);

    int index = ui->positionComboBox->findText(position, Qt::MatchFixedString);
    if (index >= 0) {
        ui->positionComboBox->setCurrentIndex(index);
    } else {
        // If position not found, default to "User"
        ui->positionComboBox->setCurrentIndex(0);
    }
}

void Settings::onSaveClicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text().trimmed();
    QString firstname = ui->firstNameLineEdit->text().trimmed();
    QString lastname = ui->lastNameLineEdit->text().trimmed();
    QString position = ui->positionComboBox->currentText().trimmed();

    if (username.isEmpty() || password.isEmpty() || firstname.isEmpty() || lastname.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "All fields except 'Position' are required.");
        return;
    }

    emit saveRequested(username, password, firstname, lastname, position);
}

void Settings::onCancelClicked()
{
    emit cancelRequested();
}

void Settings::resetUI()
{
    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();
    ui->firstNameLineEdit->clear();
    ui->lastNameLineEdit->clear();
    ui->positionComboBox->setCurrentIndex(0);
}
