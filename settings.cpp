#include "Settings.h"
#include "ui_settings.h"
#include <QMessageBox>
#include "PasswordManager.h"

Settings::Settings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Settings)
    , m_passwordManager(new PasswordManager(this))
    , m_passwordStrength(0)
    , m_passwordsMatch(false)
{
    ui->setupUi(this);

    ui->positionComboBox->clear();
    ui->positionComboBox->addItem("User");
    ui->positionComboBox->addItem("Developer");
    ui->positionComboBox->addItem("Admin");

    // Connect buttons
    connect(ui->savePushButton, &QPushButton::clicked, this, &Settings::onSaveClicked);
    connect(ui->cancelPushButton, &QPushButton::clicked, this, &Settings::onCancelClicked);

    setupPasswordValidation();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::setupPasswordValidation()
{
    // Connect password fields to PasswordManager
    connect(ui->passwordLineEdit, &QLineEdit::textChanged, m_passwordManager, &PasswordManager::setPassword);
    connect(ui->confirmPasswordLineEdit, &QLineEdit::textChanged, m_passwordManager, &PasswordManager::setConfirmPassword);

    // Connect PasswordManager signals to local slots
    connect(m_passwordManager, &PasswordManager::strengthChanged, this, &Settings::onPasswordStrengthChanged);
    connect(m_passwordManager, &PasswordManager::matchStatusChanged, this, &Settings::onPasswordMatchStatusChanged);
}

void Settings::onPasswordStrengthChanged(int strength)
{
    m_passwordStrength = strength;
    // Handle UI (e.g., update a progress bar)
}

void Settings::onPasswordMatchStatusChanged(bool match)
{
    m_passwordsMatch = match;
    // Handle UI update (e.g., display a checkmark)
}

void Settings::setUserData(const QString &username,
                           const QString &firstname, const QString &lastname, const QString &position)
{
    ui->usernameLineEdit->setText(username);
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
    QString password = ui->passwordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordLineEdit->text();
    QString firstname = ui->firstNameLineEdit->text().trimmed();
    QString lastname = ui->lastNameLineEdit->text().trimmed();
    QString position = ui->positionComboBox->currentText().trimmed();

    // Validate required fields
    if (username.isEmpty() || firstname.isEmpty() || lastname.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Username, First Name, and Last Name are required.");
        return;
    }

    bool isPasswordChanged = !password.isEmpty();
    QString finalPassword; // To hold the hashed password if changed

    if (isPasswordChanged) {
        // Confirm password is provided; check if it matches
        if (!m_passwordsMatch) {
            QMessageBox::warning(this, "Password Mismatch", "The password and confirm password fields do not match.");
            return;
        }

        // Check password strength
        if (m_passwordStrength < 60) { // Assume 60 as minimum acceptable strength
            QMessageBox::warning(this, "Weak Password", "Please choose a stronger password.");
            return;
        }

        // Passwords match and strength is sufficient; proceed to change the password
        finalPassword = m_passwordManager->hashPassword(password); // Hash the new password

        // Emit with the new hashed password
        emit saveRequested(username, finalPassword, firstname, lastname, position);
    } else {
        // Emit with an empty password to indicate no change
        emit saveRequested(username, QString(), firstname, lastname, position);
    }

    QMessageBox::information(this, "Success", "Settings have been updated successfully.");
}

void Settings::onCancelClicked()
{
    // Reset the UI to original data and emit the cancel signal
    resetUI();
    emit cancelRequested();
}

void Settings::resetUI()
{
    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();
    ui->confirmPasswordLineEdit->clear();
    ui->firstNameLineEdit->clear();
    ui->lastNameLineEdit->clear();
    ui->positionComboBox->setCurrentIndex(0);
    m_passwordStrength = 0;
    m_passwordsMatch = false;
}
