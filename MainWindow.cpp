#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <algorithm>
#include <QDebug>
#include "Transaction.h"
#include "ViewTransactions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , loginWindow(nullptr)
    , signUpWindow(nullptr)
    , transactionForm(nullptr)
    , graphView(nullptr)
    , settings(nullptr)
    , viewTransactions(nullptr)
{
    ui->setupUi(this);

    // Initialize and open the database
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("app.db");
    if (!m_db.open()) {
        qCritical() << "Failed to open database:" << m_db.lastError().text();
        QMessageBox::critical(this, "Database Error", m_db.lastError().text());
        return;
    }

    // Ensure necessary tables exist
    {
        QSqlQuery query(m_db);
        if (!query.exec("CREATE TABLE IF NOT EXISTS User ("
                        "userID INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "firstname TEXT NOT NULL,"
                        "lastname TEXT NOT NULL,"
                        "position TEXT NOT NULL)")) {
            QMessageBox::critical(this, "DB Error", query.lastError().text());
        }

        if (!query.exec("CREATE TABLE IF NOT EXISTS UserLogin ("
                        "loginID INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "username TEXT UNIQUE NOT NULL,"
                        "password TEXT NOT NULL,"
                        "accessLevel INTEGER NOT NULL,"
                        "userID INTEGER NOT NULL,"
                        "FOREIGN KEY(userID) REFERENCES User(userID))")) {
            QMessageBox::critical(this, "DB Error", query.lastError().text());
        }

        // Transactions table
        if (!query.exec("CREATE TABLE IF NOT EXISTS transactions ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "userId INTEGER NOT NULL, "
                        "date TEXT NOT NULL, "
                        "category TEXT NOT NULL, "
                        "subcategory TEXT, "
                        "amount REAL NOT NULL, "
                        "type TEXT NOT NULL, "
                        "taxWithheld INTEGER NOT NULL DEFAULT 0, "
                        "taxAmount REAL NOT NULL DEFAULT 0.0, "
                        "FOREIGN KEY(userId) REFERENCES User(userID))")) {
            qCritical() << "Failed to create transactions table:" << query.lastError().text();
        }
    }

    // Instantiate widgets
    loginWindow = new LoginWindow(m_db, this);
    signUpWindow = new SignUpWindow(m_db, this);
    transactionForm = new TransactionForm(this);
    graphView = new GraphView(this);
    settings = new Settings(this);
    viewTransactions = new ViewTransactions(this);

    // Add them to the stacked widget
    ui->stackedWidget->addWidget(loginWindow);
    ui->stackedWidget->addWidget(signUpWindow);
    ui->stackedWidget->addWidget(transactionForm);
    ui->stackedWidget->addWidget(graphView);
    ui->stackedWidget->addWidget(settings);
    ui->stackedWidget->addWidget(viewTransactions);

    // Initial screen is the login window
    ui->stackedWidget->setCurrentWidget(loginWindow);
    setWindowTitle("Log In");

    // Set up navigation combo box
    ui->navComboBox->clear();
    ui->navComboBox->addItem("View Transactions");
    ui->navComboBox->addItem("View Graphs");
    ui->navComboBox->addItem("Add Transaction");
    ui->navComboBox->addItem("Settings");
    ui->navComboBox->addItem("Logout");

    // Navigation Signal
    connect(ui->navComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onNavComboBoxChanged);

    // Log in/Sign up signals
    connect(loginWindow, &LoginWindow::showSignUp, this, &MainWindow::showSignUpWindow);
    connect(loginWindow, &LoginWindow::loginSuccess, this, &MainWindow::onLoginSuccess);
    connect(signUpWindow, &SignUpWindow::showLogin, this, &MainWindow::showLoginWindow);

    // Transaction signals
    connect(transactionForm, &TransactionForm::transactionSaved, this, &MainWindow::reloadLedger);
    connect(transactionForm, &TransactionForm::transactionSaved, this, &MainWindow::showViewTransactions);
    connect(transactionForm, &TransactionForm::transactionCancelled, this, &MainWindow::showViewTransactions);

    // Settings signals
    connect(settings, &Settings::saveRequested, this, &MainWindow::onSettingsSaved);
    connect(settings, &Settings::cancelRequested, this, &MainWindow::onSettingsCancelled);

    updateNavVisibility();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoginSuccess(const QString &firstname, const QString &lastname, const QString &position)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT userID FROM User WHERE firstname=? AND lastname=? AND position=? ORDER BY userID DESC LIMIT 1");
    query.addBindValue(firstname);
    query.addBindValue(lastname);
    query.addBindValue(position);
    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Error", "Failed to identify user.");
        return;
    }
    int userID = query.value(0).toInt();

    currentUser = User(userID, firstname, lastname, position);

    // Load this user's transactions
    ledger.clear();
    std::vector<Transaction> allTransactions = Transaction::readAllTransactions();
    for (const auto &t : allTransactions) {
        if (t.getUserId() == currentUser.getUserId()) {
            ledger.addTransaction(t);
        }
    }

    transactionForm->setCurrentUser(currentUser);

    viewTransactions->setCurrentUser(currentUser);
    viewTransactions->setAllTransactions(ledger.getAllTransactions());

    graphView->setCurrentUser(currentUser);
    graphView->setAllTransactions(ledger.getAllTransactions());

    showViewTransactions();
}

void MainWindow::showSignUpWindow()
{
    // Reset UI of log in window when leaving it
    loginWindow->resetUI();
    signUpWindow->resetUI();
    ui->stackedWidget->setCurrentWidget(signUpWindow);
    setWindowTitle("Sign Up");
    updateNavVisibility();
}

void MainWindow::showLoginWindow()
{
    // Reset UI of sign up window when leaving it
    signUpWindow->resetUI();
    loginWindow->resetUI();
    ui->stackedWidget->setCurrentWidget(loginWindow);
    setWindowTitle("Log In");
    updateNavVisibility();
}

void MainWindow::showTransactionForm()
{
    // Reset UI of other forms when leaving them
    viewTransactions->resetUI();
    graphView->resetUI();
    settings->resetUI();
    transactionForm->resetUI();

    ui->stackedWidget->setCurrentWidget(transactionForm);
    setWindowTitle("Add Transaction");
    updateNavVisibility();
}

void MainWindow::showViewTransactions()
{
    // Reset UI of other forms
    transactionForm->resetUI();
    graphView->resetUI();
    settings->resetUI();
    viewTransactions->resetUI();

    ui->stackedWidget->setCurrentWidget(viewTransactions);
    setWindowTitle("View Transactions");

    ui->navComboBox->blockSignals(true);
    ui->navComboBox->setCurrentText("View Transactions");
    ui->navComboBox->blockSignals(false);

    updateNavVisibility();
}

void MainWindow::showGraphView()
{
    // Reset UI of other forms
    transactionForm->resetUI();
    viewTransactions->resetUI();
    settings->resetUI();
    graphView->resetUI();

    ui->stackedWidget->setCurrentWidget(graphView);
    setWindowTitle("View Graphs");

    ui->navComboBox->blockSignals(true);
    ui->navComboBox->setCurrentText("View Graphs");
    ui->navComboBox->blockSignals(false);

    updateNavVisibility();
}

void MainWindow::showSettings()
{
    // Reset UI of other forms
    transactionForm->resetUI();
    viewTransactions->resetUI();
    graphView->resetUI();
    settings->resetUI();

    populateSettingsWithCurrentUser();
    ui->stackedWidget->setCurrentWidget(settings);
    setWindowTitle("Settings");
    updateNavVisibility();
}

QVector<QPointF> MainWindow::getDataPointsForGraph()
{
    QVector<QPointF> dataPoints;
    auto userTransactions = ledger.getAllTransactions();

    if (userTransactions.empty()) {
        return dataPoints;
    }

    std::sort(userTransactions.begin(), userTransactions.end(), [](const Transaction &a, const Transaction &b) {
        return a.getDate() < b.getDate();
    });

    double balance = 0.0;
    for (const auto &transaction : userTransactions) {
        QDateTime transactionDateTime = QDateTime::fromString(QString::fromStdString(transaction.getDate()), "yyyy-MM-dd");
        if (!transactionDateTime.isValid()) {
            qWarning() << "Invalid transaction date:" << QString::fromStdString(transaction.getDate());
            continue;
        }

        if (transaction.isIncomeTransaction()) {
            balance += transaction.getAmount();
        } else {
            balance -= transaction.getAmount();
        }

        dataPoints.append(QPointF(transactionDateTime.toMSecsSinceEpoch(), balance));
    }

    return dataPoints;
}

void MainWindow::reloadLedger()
{
    ledger.clear();
    std::vector<Transaction> allTransactions = Transaction::readAllTransactions();
    for (const auto &t : allTransactions) {
        if (t.getUserId() == currentUser.getUserId()) {
            ledger.addTransaction(t);
        }
    }

    viewTransactions->setAllTransactions(ledger.getAllTransactions());
    graphView->setAllTransactions(ledger.getAllTransactions());
}

void MainWindow::onNavComboBoxChanged(const QString &text)
{
    static bool isHandling = false;
    if (isHandling)
        return;

    isHandling = true;

    if (text == "Add Transaction") {
        showTransactionForm();
    } else if (text == "View Transactions") {
        showViewTransactions();
    } else if (text == "View Graphs") {
        showGraphView();
    } else if (text == "Settings") {
        showSettings();
    } else if (text == "Logout") {
        // Reset UI when logging out
        transactionForm->resetUI();
        graphView->resetUI();
        settings->resetUI();
        viewTransactions->resetUI();
        loginWindow->resetUI();
        signUpWindow->resetUI();

        currentUser = User();
        ledger.clear();
        showLoginWindow();

        ui->navComboBox->blockSignals(true);
        ui->navComboBox->setCurrentText("View Transactions");
        ui->navComboBox->blockSignals(false);
    }

    isHandling = false;
}

void MainWindow::onSettingsSaved(const QString &username, const QString &password,
                                 const QString &firstName, const QString &lastName, const QString &position)
{
    if (updateUserInDatabase(firstName, lastName, position, username, password)) {
        currentUser.setFirstName(firstName);
        currentUser.setLastName(lastName);
        currentUser.setPosition(position);
        showViewTransactions();
    } else {
        QMessageBox::warning(this, "Error", "Failed to update user details. Please try again.");
    }
}

void MainWindow::onSettingsCancelled()
{
    showViewTransactions();
}

void MainWindow::updateNavVisibility()
{
    QWidget *currentWidget = ui->stackedWidget->currentWidget();
    if (currentWidget == loginWindow || currentWidget == signUpWindow) {
        ui->navComboBox->hide();
    } else {
        ui->navComboBox->show();
    }
}


bool MainWindow::updateUserInDatabase(const QString &firstName, const QString &lastName, const QString &position,
                                      const QString &username, const QString &password)
{
    if (currentUser.getUserId() == 0) {
        // No user logged in
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("UPDATE User SET firstname=?, lastname=?, position=? WHERE userID=?");
    query.addBindValue(firstName);
    query.addBindValue(lastName);
    query.addBindValue(position);
    query.addBindValue(currentUser.getUserId());

    if (!query.exec()) {
        qWarning() << "Failed to update user table:" << query.lastError().text();
        return false;
    }

    // Update UserLogin info conditionally based on whether a new password was provided
    QSqlQuery loginQuery(m_db);
    if (!password.isEmpty()) {
        // If password is provided, update both username and password
        loginQuery.prepare("UPDATE UserLogin SET username=?, password=? WHERE userID=?");
        loginQuery.addBindValue(username);
        loginQuery.addBindValue(password); // Hashed password
    } else {
        // If password is empty, update only the username
        loginQuery.prepare("UPDATE UserLogin SET username=? WHERE userID=?");
        loginQuery.addBindValue(username);
    }
    loginQuery.addBindValue(currentUser.getUserId());

    if (!loginQuery.exec()) {
        qWarning() << "Failed to update user login:" << loginQuery.lastError().text();
        return false;
    }

    return true;
}

void MainWindow::populateSettingsWithCurrentUser()
{
    QSqlQuery query(m_db);
    query.prepare("SELECT UserLogin.username, UserLogin.password, User.firstname, User.lastname, User.position "
                  "FROM UserLogin JOIN User ON UserLogin.userID=User.userID "
                  "WHERE User.userID=?");
    query.addBindValue(currentUser.getUserId());

    if (query.exec() && query.next()) {
        QString username = query.value("username").toString();
        QString firstname = query.value("firstname").toString();
        QString lastname = query.value("lastname").toString();
        QString position = query.value("position").toString();

        settings->setUserData(username, firstname, lastname, position);
    }
    else {
        qWarning() << "Failed to load user data for settings:" << query.lastError().text();
    }
}
