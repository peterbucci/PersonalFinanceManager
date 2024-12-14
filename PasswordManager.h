#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include <QObject>
#include <QString>

/**
 * @brief The PasswordManager class handles password hashing and strength validation.
 */
class PasswordManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the PasswordManager.
     * @param parent The parent QObject.
     */
    explicit PasswordManager(QObject *parent = nullptr);

    /**
     * @brief Hashes the given plain-text password using SHA-256.
     * @param password The plain-text password to hash.
     * @return The hashed password as a hexadecimal QString.
     */
    QString hashPassword(const QString &password);

    /**
     * @brief Retrieves the current password strength.
     * @return An integer representing the strength level (0-100).
     */
    int getPasswordStrength() const;

    /**
     * @brief Checks if the password and confirm password match.
     * @return True if they match, false otherwise.
     */
    bool doPasswordsMatch() const;

public slots:
    /**
     * @brief Sets the password and updates its strength.
     * @param password The password entered by the user.
     */
    void setPassword(const QString &password);

    /**
     * @brief Sets the confirm password and updates the match status.
     * @param confirmPassword The confirm password entered by the user.
     */
    void setConfirmPassword(const QString &confirmPassword);

signals:
    /**
     * @brief Emitted when the password strength changes.
     * @param strength The new strength value.
     */
    void strengthChanged(int strength);

    /**
     * @brief Emitted when the password match status changes.
     * @param match True if passwords match, false otherwise.
     */
    void matchStatusChanged(bool match);

private:
    /**
     * @brief Evaluates the strength of the current password.
     */
    void evaluateStrength();

    /**
     * @brief Evaluates whether the password and confirm password match.
     */
    void evaluateMatch();

    QString m_password;          ///< The current password.
    QString m_confirmPassword;   ///< The current confirm password.
    int m_strength;              ///< Current password strength.
    bool m_passwordsMatch;       ///< Current password match status.
};

#endif // PASSWORDMANAGER_H
