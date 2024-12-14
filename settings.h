#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "PasswordManager.h"

namespace Ui {
class Settings;
}

/**
 * @brief The Settings class allows editing the current user's account information.
 */
class Settings : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the Settings widget.
     * @param parent Parent widget.
     */
    explicit Settings(QWidget *parent = nullptr);

    /**
     * @brief Destructs the Settings widget.
     */
    ~Settings();

    /**
     * @brief Populates the settings fields with given user data.
     * @param username The user's username.
     * @param firstname The user's first name.
     * @param lastname The user's last name.
     * @param position The user's position.
     */
    void setUserData(const QString &username,
                     const QString &firstname, const QString &lastname, const QString &position);

    /**
     * @brief Resets all UI elements to their default state.
     */
    void resetUI();

signals:
    /**
     * @brief Emitted when the user clicks 'Save'.
     * @param username The updated username.
     * @param password The updated password (hashed if changed).
     * @param firstname The updated first name.
     * @param lastname The updated last name.
     * @param position The updated position.
     */
    void saveRequested(const QString &username, const QString &password,
                       const QString &firstname, const QString &lastname, const QString &position);

    /**
     * @brief Emitted when the user clicks 'Cancel'.
     */
    void cancelRequested();

private slots:
    /**
     * @brief Handles Save button click.
     */
    void onSaveClicked();

    /**
     * @brief Handles Cancel button click.
     */
    void onCancelClicked();

    /**
     * @brief Slot to handle updates to password strength.
     * @param strength The new password strength value.
     */
    void onPasswordStrengthChanged(int strength);

    /**
     * @brief Slot to handle updates to password match status.
     * @param match True if passwords match, false otherwise.
     */
    void onPasswordMatchStatusChanged(bool match);

private:
    Ui::Settings *ui; ///< Pointer to the UI components of Settings.
    PasswordManager *m_passwordManager; ///< Manages password hashing and validation
    int m_passwordStrength; ///< Current password strength
    bool m_passwordsMatch; ///< Current password match status

    /**
     * @brief Sets up connections for password validation.
     */
    void setupPasswordValidation();
};

#endif // SETTINGS_H
