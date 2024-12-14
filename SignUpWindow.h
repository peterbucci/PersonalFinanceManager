#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QWidget>
#include <QSqlDatabase>
#include "PasswordManager.h"

namespace Ui {
class SignUpWindow;
}

/**
 * @brief The SignUpWindow class provides the UI and logic for user registration.
 */
class SignUpWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the SignUpWindow with a specified database connection.
     * @param db The database connection reference.
     * @param parent The parent widget.
     */
    explicit SignUpWindow(QSqlDatabase db, QWidget *parent = nullptr);

    /**
     * @brief Default constructor for SignUpWindow.
     */
    SignUpWindow();

    /**
     * @brief Destructs the SignUpWindow.
     */
    ~SignUpWindow();

    /**
     * @brief Resets all UI elements to their default state.
     */
    void resetUI();

signals:
    /**
     * @brief Emitted when the user clicks to show the Login window.
     */
    void showLogin();

private slots:
    /**
     * @brief Handles the sign up button click.
     *
     * This slot validates user input, ensuring that all required fields are filled,
     * and that the password and confirm password fields match. If validation passes,
     * it inserts the new user into the database. Appropriate error messages are displayed
     * if validation fails or if database operations encounter issues.
     */
    void on_signUpPushButton_clicked();

    /**
     * @brief Slot to handle updates to password strength.
     * @param strength The new strength value.
     */
    void onPasswordStrengthChanged(int strength);

    /**
     * @brief Slot to handle updates to password match status.
     * @param match True if passwords match, false otherwise.
     */
    void onPasswordMatchStatusChanged(bool match);

protected:
    /**
     * @brief Overrides the event filter to handle specific events.
     * @param obj The object where the event originated.
     * @param event The event to be filtered.
     * @return true if the event is handled, false otherwise.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    /**
     * @brief Sets up connections for password validation.
     */
    void setupPasswordValidation();


    Ui::SignUpWindow *ui;                ///< Pointer to the UI components of SignUpWindow.
    QSqlDatabase m_db;                   ///< Database connection reference.
    PasswordManager *m_passwordManager;  ///< Manages password hashing and validation
    int m_passwordStrength;              ///< Current password strength
    bool m_passwordsMatch;               ///< Current password match status
};

#endif // SIGNUPWINDOW_H
