#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include "LoginWindow.h"
#include "SignUpWindow.h"
#include "TransactionForm.h"
#include "GraphView.h"
#include "Settings.h"
#include "ViewTransactions.h"
#include "User.h"
#include "Ledger.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class represents the main application window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the MainWindow.
     * @param parent The parent widget.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructs the MainWindow.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Handles successful login, given the user's firstname, lastname, and position.
     * @param firstname The user's first name.
     * @param lastname The user's last name.
     * @param position The user's position.
     */
    void onLoginSuccess(const QString &firstname, const QString &lastname, const QString &position);

    /**
     * @brief Shows the SignUpWindow.
     */
    void showSignUpWindow();

    /**
     * @brief Returns to LoginWindow from SignUpWindow.
     */
    void showLoginWindow();

    /**
     * @brief Shows the form for adding a new transaction.
     */
    void showTransactionForm();

    /**
     * @brief Shows the graph view screen.
     */
    void showGraphView();

    /**
     * @brief Shows the View Transactions page.
     */
    void showViewTransactions(); // ADDED

    /**
     * @brief Shows the settings page.
     */
    void showSettings();

    /**
     * @brief Gets data points for the graph view.
     * @return A QVector of data points.
     */
    QVector<QPointF> getDataPointsForGraph();

    /**
     * @brief Reloads the Ledger after a new transaction is added.
     */
    void reloadLedger();

    /**
     * @brief Handles navigation combo box changes.
     * @param text The current text of the navigation combo box.
     */
    void onNavComboBoxChanged(const QString &text);

    /**
     * @brief Called when the user saves changes in the Settings.
     * Updates the user record in the database and returns to View Transactions.
     * @param username The updated username.
     * @param password The updated password.
     * @param firstName The updated first name.
     * @param lastName The updated last name.
     * @param position The updated position.
     */
    void onSettingsSaved(const QString &username, const QString &password,
                         const QString &firstName, const QString &lastName, const QString &position);

    /**
     * @brief Called when the user cancels changes in the Settings.
     * Returns to the View Transactions without saving.
     */
    void onSettingsCancelled();

private:
    Ui::MainWindow *ui; ///< Pointer to the UI components of MainWindow.
    QSqlDatabase m_db; ///< Database connection reference.
    User currentUser; ///< The currently logged-in user.
    LoginWindow *loginWindow; ///< Pointer to the LoginWindow.
    SignUpWindow *signUpWindow; ///< Pointer to the SignUpWindow.
    TransactionForm *transactionForm; ///< Pointer to the TransactionForm.
    GraphView *graphView; ///< Pointer to the GraphView.
    Settings *settings; ///< Pointer to the Settings.
    ViewTransactions *viewTransactions; ///< Pointer to the ViewTransactions.
    Ledger ledger; ///< Ledger object managing financial transactions.

    /**
     * @brief Updates the visibility of the navigation combo box based on the current page.
     */
    void updateNavVisibility();

    /**
     * @brief Updates the user details in the database.
     * @param firstName Updated first name.
     * @param lastName Updated last name.
     * @param position Updated position.
     * @param username Updated username.
     * @param password Updated password.
     * @return true if updated successfully, false otherwise.
     */
    bool updateUserInDatabase(const QString &firstName, const QString &lastName, const QString &position,
                              const QString &username, const QString &password);

    /**
     * @brief Populates the Settings UI with the current user's data.
     */
    void populateSettingsWithCurrentUser();
};

#endif // MAINWINDOW_H
