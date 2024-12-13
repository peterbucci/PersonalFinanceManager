#include "SignUpWindow.h"
#include "ui_SignUpWindow.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
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
    m_db(db)
{
    ui->setupUi(this);
    ui->positionComboBox->addItems(QStringList() << "User" << "Developer" << "Admin");

    ui->logInLink->setCursor(Qt::PointingHandCursor);
    ui->logInLink->installEventFilter(this);

    this->setWindowTitle("Sign Up");
}

SignUpWindow::~SignUpWindow()
{
    delete ui;
}

void SignUpWindow::on_signUpPushButton_clicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text().trimmed();
    QString firstname = ui->firstNameLineEdit->text().trimmed();
    QString lastname = ui->lastNameLineEdit->text().trimmed();
    QString position = ui->positionComboBox->currentText();

    if (username.isEmpty() || password.isEmpty() || firstname.isEmpty() || lastname.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields are required.");
        return;
    }

    User newUser(0, firstname, lastname, position);

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

    UserLogin newUserLogin(0, username, password, accessLevel, newUser.getUserId());

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
        QMessageBox::warning(this, "Error", "Username may already exist. " + query2.lastError().text());
        return;
    }

    QMessageBox::information(this, "Success", "User registered successfully. Please log in.");
    emit showLogin();
}

bool SignUpWindow::eventFilter(QObject *obj, QEvent *event)
{
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
    ui->firstNameLineEdit->clear();
    ui->lastNameLineEdit->clear();
    ui->positionComboBox->setCurrentIndex(0);
}
