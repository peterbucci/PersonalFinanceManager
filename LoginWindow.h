#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class LoginWindow;
}

/**
 * @brief The LoginWindow class provides the UI and logic for user login.
 */
class LoginWindow : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Constructs the LoginWindow.
     */
    LoginWindow();

    /**
     * @brief Constructs the LoginWindow.
     * @param db The database connection reference.
     * @param parent The parent widget.
     * @return A LoginWindow object.
     */
    explicit LoginWindow(QSqlDatabase db, QWidget *parent = nullptr);

    /**
     * @brief Destructs the LoginWindow.
     */
    ~LoginWindow();

    /**
     * @brief Resets all UI elements to their default state.
     */
    void resetUI();

signals:
    /**
     * @brief Emitted when login is successful.
     * @param firstname The user's first name.
     * @param lastname The user's last name.
     * @param position The user's position.
     */
    void loginSuccess(const QString &firstname, const QString &lastname, const QString &position);

    /**
     * @brief Emitted when the user clicks to show the Sign Up window.
     */
    void showSignUp();

private slots:
    /**
     * @brief Attempts to log in the user when the logInPushButton is clicked.
     */
    void on_logInPushButton_clicked();

private:
    /**
     * @brief Overrides the event filter to handle specific events.
     * @param obj The object where the event originated.
     * @param event The event to be filtered.
     * @return true if the event is handled, false otherwise.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;

    /**
     * @brief Handles the Forgot Password button click event.
     */
    void forgotPasswordClicked();

    /**
     * @brief Saves user credentials securely.
     */
    void saveCredentials();

    /**
     * @brief Loads saved user credentials.
     */
    void loadCredentials();

    Ui::LoginWindow *ui; ///< Pointer to the UI components of LoginWindow.
    QSqlDatabase m_db; ///< Database connection reference.
};

#endif // LOGINWINDOW_H
