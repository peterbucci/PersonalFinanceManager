#include "loginwindow.h"
#include "ui_LoginWindow.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QSettings>
#include "user.h"
#include "userlogin.h"

LoginWindow::LoginWindow() {}

LoginWindow::LoginWindow(QSqlDatabase db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow),
    m_db(db)
{
    ui->setupUi(this);

    // Enable clickable links
    ui->forgotLogInLink->setCursor(Qt::PointingHandCursor);
    ui->forgotLogInLink->installEventFilter(this);
    ui->signUpLink->setCursor(Qt::PointingHandCursor);
    ui->signUpLink->installEventFilter(this);

    // Load remembered credentials if any
    loadCredentials();

    this->setWindowTitle("Log In");
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_logInPushButton_clicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Username and password cannot be empty.");
        return;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT loginID, userID, accessLevel FROM UserLogin WHERE username=? AND password=?");
    query.addBindValue(username);
    query.addBindValue(password);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Database query error: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        int loginID = query.value("loginID").toInt();
        int userID = query.value("userID").toInt();
        int accessLevel = query.value("accessLevel").toInt();

        UserLogin userLogin(loginID, username, password, accessLevel, userID);

        QSqlQuery userQuery(m_db);
        userQuery.prepare("SELECT firstname, lastname, position FROM User WHERE userID=?");
        userQuery.addBindValue(userLogin.getUserId());

        if (!userQuery.exec()) {
            QMessageBox::critical(this, "Error", "Database query error: " + userQuery.lastError().text());
            return;
        }

        if (userQuery.next()) {
            QString firstname = userQuery.value("firstname").toString();
            QString lastname  = userQuery.value("lastname").toString();
            QString position  = userQuery.value("position").toString();

            User user(userLogin.getUserId(), firstname, lastname, position);

            // If remember me is checked, save credentials
            saveCredentials();

            emit loginSuccess(user.getFirstName(), user.getLastName(), user.getPosition());
        } else {
            QMessageBox::warning(this, "Error", "User data not found for this userID.");
        }
    } else {
        QMessageBox::warning(this, "Error", "Invalid username or password. Please try again.");
        ui->usernameLineEdit->clear();
        ui->passwordLineEdit->clear();
    }
}

bool LoginWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->forgotLogInLink && event->type() == QEvent::MouseButtonRelease) {
        forgotPasswordClicked();
        return true;
    } else if (obj == ui->signUpLink && event->type() == QEvent::MouseButtonRelease) {
        emit showSignUp();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void LoginWindow::forgotPasswordClicked()
{
    bool ok;
    QString username = QInputDialog::getText(this, "Forgot Password", "Enter your username:", QLineEdit::Normal, "", &ok);
    if (!ok || username.trimmed().isEmpty()) {
        return; // user cancelled
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT loginID FROM UserLogin WHERE username=?");
    query.addBindValue(username.trimmed());

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Query failed: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        QString newPassword = "temp123";
        QSqlQuery updateQuery(m_db);
        updateQuery.prepare("UPDATE UserLogin SET password=? WHERE username=?");
        updateQuery.addBindValue(newPassword);
        updateQuery.addBindValue(username.trimmed());

        if (!updateQuery.exec()) {
            QMessageBox::critical(this, "Error", "Failed to reset password: " + updateQuery.lastError().text());
            return;
        }

        QMessageBox::information(this, "Password Reset", "Your new password is: " + newPassword);
    } else {
        QMessageBox::warning(this, "Error", "Username does not exist.");
    }
}

void LoginWindow::saveCredentials()
{
    QSettings settings("Crumpet", "Unit13RA");
    if (ui->rememberMeCheckBox->isChecked()) {
        settings.setValue("username", ui->usernameLineEdit->text());
        settings.setValue("password", ui->passwordLineEdit->text());
    } else {
        settings.remove("username");
        settings.remove("password");
    }
}

void LoginWindow::loadCredentials()
{
    QSettings settings("Crumpet", "Unit13RA");
    QString savedUsername = settings.value("username", "").toString();
    QString savedPassword = settings.value("password", "").toString();

    if (!savedUsername.isEmpty() && !savedPassword.isEmpty()) {
        ui->usernameLineEdit->setText(savedUsername);
        ui->passwordLineEdit->setText(savedPassword);
        ui->rememberMeCheckBox->setChecked(true);
    }
}

void LoginWindow::resetUI()
{
    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();
    ui->rememberMeCheckBox->setChecked(false);
    // Load remembered credentials if any
    loadCredentials();
}
