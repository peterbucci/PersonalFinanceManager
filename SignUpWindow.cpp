#include "SignUpWindow.h"
#include "ui_SignUpWindow.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QToolTip>
#include <QFocusEvent>
#include <QCursor>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include "user.h"
#include "userlogin.h"

static int getAccessLevel(const QString &position) {
    QString pos = position.toLower();
    if (pos == "admin") return 3;
    if (pos == "developer") return 2;
    return 1; // Default lowest access
}

SignUpWindow::SignUpWindow(QSqlDatabase db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUpWindow),
    m_db(db),
    m_passwordManager(new PasswordManager(this)),
    m_passwordStrength(0),
    m_passwordsMatch(false)
{
    ui->setupUi(this);
    ui->positionComboBox->addItems(QStringList() << "User" << "Developer" << "Admin");

    ui->logInLink->setCursor(Qt::PointingHandCursor);
    ui->logInLink->installEventFilter(this);

    this->setWindowTitle("Sign Up");

    setupPasswordValidation();

    // Install event filters on password fields to handle focus events
    ui->passwordLineEdit->installEventFilter(this);
    ui->confirmPasswordLineEdit->installEventFilter(this);

    // Initialize custom tooltip via PasswordManager
    m_passwordManager->initializeTooltip(this);
}

// Default Constructor
SignUpWindow::SignUpWindow(){}

SignUpWindow::~SignUpWindow()
{
    delete ui;
}

void SignUpWindow::setupPasswordValidation()
{
    // Connect password fields to PasswordManager
    connect(ui->passwordLineEdit, &QLineEdit::textChanged, m_passwordManager, &PasswordManager::setPassword);
    connect(ui->confirmPasswordLineEdit, &QLineEdit::textChanged, m_passwordManager, &PasswordManager::setConfirmPassword);

    // Connect PasswordManager signals to local slots
    connect(m_passwordManager, &PasswordManager::strengthChanged, this, &SignUpWindow::onPasswordStrengthChanged);
    connect(m_passwordManager, &PasswordManager::matchStatusChanged, this, &SignUpWindow::onPasswordMatchStatusChanged);
}

void SignUpWindow::onPasswordStrengthChanged(int strength)
{
    m_passwordStrength = strength;

    // If tooltip is visible on password fields, update it via PasswordManager
    if (ui->passwordLineEdit->hasFocus() || ui->confirmPasswordLineEdit->hasFocus()) {
        // Always position the tooltip below confirmPasswordLineEdit
        m_passwordManager->showPasswordTooltip(ui->confirmPasswordLineEdit);
    }
}

void SignUpWindow::onPasswordMatchStatusChanged(bool match)
{
    m_passwordsMatch = match;

    // If tooltip is visible on password fields, update it via PasswordManager
    if (ui->passwordLineEdit->hasFocus() || ui->confirmPasswordLineEdit->hasFocus()) {
        // Always position the tooltip below confirmPasswordLineEdit
        m_passwordManager->showPasswordTooltip(ui->confirmPasswordLineEdit);
    }
}

void SignUpWindow::on_signUpPushButton_clicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordLineEdit->text();
    QString firstname = ui->firstNameLineEdit->text().trimmed();
    QString lastname = ui->lastNameLineEdit->text().trimmed();
    QString position = ui->positionComboBox->currentText();

    // Validate required fields
    if (username.isEmpty() || password.isEmpty() || firstname.isEmpty() || lastname.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields are required.");
        return;
    }

    // Check if password and confirm password match
    if (!m_passwordsMatch) {
        QMessageBox::warning(this, "Password Mismatch", "The password and confirm password fields do not match.");
        return;
    }

    // Check password strength
    if (m_passwordStrength < 60) { // 60 is the minimum acceptable strength
        QMessageBox::warning(this, "Weak Password", "Please choose a stronger password.");
        return;
    }

    // Hash the password before storing
    QString hashedPassword = m_passwordManager->hashPassword(password);

    // Create a new User object
    User newUser(0, firstname, lastname, position);

    // Insert the new user into the User table
    QSqlQuery query(m_db);
    if (!query.prepare("INSERT INTO User (firstname, lastname, position) VALUES (?, ?, ?)")) {
        QMessageBox::critical(this, "Error", "Failed to prepare user insert: " + query.lastError().text());
        return;
    }
    query.addBindValue(newUser.getFirstName());
    query.addBindValue(newUser.getLastName());
    query.addBindValue(newUser.getPosition());

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to insert user: " + query.lastError().text());
        return;
    }

    int userId = query.lastInsertId().toInt();
    newUser.setUserId(userId);
    int accessLevel = getAccessLevel(newUser.getPosition());

    // Create a new UserLogin object
    UserLogin newUserLogin(0, username, hashedPassword, accessLevel, newUser.getUserId());

    // Insert the new user login into the UserLogin table
    QSqlQuery query2(m_db);
    if (!query2.prepare("INSERT INTO UserLogin (username, password, accessLevel, userID) VALUES (?, ?, ?, ?)")) {
        QMessageBox::critical(this, "Error", "Failed to prepare login insert: " + query2.lastError().text());
        return;
    }
    query2.addBindValue(newUserLogin.getUsername());
    query2.addBindValue(newUserLogin.getPassword());
    query2.addBindValue(newUserLogin.getAccessLevel());
    query2.addBindValue(newUserLogin.getUserId());

    if (!query2.exec()) {
        QMessageBox::warning(this, "Error", "Username already exists. " + query2.lastError().text());
        return;
    }

    QMessageBox::information(this, "Success", "User registered successfully. Please log in.");
    emit showLogin();
}

bool SignUpWindow::eventFilter(QObject *obj, QEvent *event)
{
    // Handle focus events for password fields to show/hide tooltips
    if ((obj == ui->passwordLineEdit || obj == ui->confirmPasswordLineEdit)) {
        if (event->type() == QEvent::FocusIn) {
            // Always position the tooltip below confirmPasswordLineEdit
            m_passwordManager->showPasswordTooltip(ui->confirmPasswordLineEdit);
        }
        else if (event->type() == QEvent::FocusOut) {
            // Delay hiding to allow focus to move to the other password field if applicable
            QTimer::singleShot(100, this, [this]() {
                if (!ui->passwordLineEdit->hasFocus() && !ui->confirmPasswordLineEdit->hasFocus()) {
                    m_passwordManager->hidePasswordTooltip();
                }
            });
        }
    }

    // Handle mouse events for logInLink
    if (obj == ui->logInLink && event->type() == QEvent::MouseButtonRelease) {
        emit showLogin();
        return true;
    }

    return QWidget::eventFilter(obj, event);
}

void SignUpWindow::resetUI()
{
    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();
    ui->confirmPasswordLineEdit->clear();
    ui->firstNameLineEdit->clear();
    ui->lastNameLineEdit->clear();
    ui->positionComboBox->setCurrentIndex(0);
    m_passwordStrength = 0;
    m_passwordsMatch = false;
    m_passwordManager->hidePasswordTooltip();
}
